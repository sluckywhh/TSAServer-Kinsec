#include "http_conn.h"

const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to satisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The requested file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the requested file.\n";

const char* file_root_path = "../File/";

int setNonBlock( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd, bool one_shot )
{
    epoll_event event;
    event.data.fd = fd;
    //event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;//EPOLLET：将EPOLL设为边缘触发(Edge Triggered)模式
    event.events = EPOLLIN | EPOLLRDHUP;
    if( one_shot )
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setNonBlock( fd );
}

void removefd( int epollfd, int fd )
{
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
    close( fd );
}

void modfd( int epollfd, int fd, int ev )
{
    epoll_event event;
    event.data.fd = fd;
    //event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}

int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

void http_conn::close_conn( bool real_close )
{
    if( real_close && ( m_socketfd != -1 ) )
    {
        //modfd( m_epollfd, m_socketfd, EPOLLIN );
        removefd( m_epollfd, m_socketfd );
        m_socketfd = -1;
        m_user_count--;
    }
}

void http_conn::init( int socketfd, const sockaddr_in& addr )
{
    printf( "[Info] user init: socketfd[%d], ipaddr[%s] \n", socketfd, inet_ntoa(addr.sin_addr) );

    m_socketfd = socketfd;
    m_address = addr;
    /*
    int error = 0;
    socklen_t len = sizeof( error );
    getsockopt( m_socketfd, SOL_SOCKET, SO_ERROR, &error, &len );*/
    int reuse = 1;
    setsockopt( m_socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    
    addfd( m_epollfd, socketfd, true );
    m_user_count++;

    init();
}

void http_conn::init()
{
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_linger = false;

    m_method = GET;
    m_url = 0;
    m_version = 0;
    m_user_agent = 0;
    m_accept = 0;
    m_request_body = 0;
    m_content_type_request = 0;
    m_content_length_request = 0;
    m_content_type_response = 0;
    m_content_length_response = 0;
    m_function = 0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    
    memset( m_response_body, '\0', sizeof(m_response_body) );
    memset( m_read_buf, '\0', READ_BUFFER_SIZE );
    memset( m_write_buf, '\0', WRITE_BUFFER_SIZE );
    memset( m_real_file, '\0', FILENAME_LEN );
}

http_conn::LINE_STATUS http_conn::parse_line()
{
    char temp;
    for ( ; m_checked_idx < m_read_idx; ++m_checked_idx )
    {
        temp = m_read_buf[ m_checked_idx ];
        if ( temp == '\r' )
        {
            if ( ( m_checked_idx + 1 ) == m_read_idx )
            {
                return LINE_OPEN;
            }
            else if ( m_read_buf[ m_checked_idx + 1 ] == '\n' )
            {
                m_read_buf[ m_checked_idx++ ] = '\0';
                m_read_buf[ m_checked_idx++ ] = '\0';
                return LINE_OK;
            }

            return LINE_BAD;
        }
        else if( temp == '\n' )
        {
            if( ( m_checked_idx > 1 ) && ( m_read_buf[ m_checked_idx - 1 ] == '\r' ) )
            {
                m_read_buf[ m_checked_idx-1 ] = '\0';
                m_read_buf[ m_checked_idx++ ] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }

    return LINE_OPEN;
}

bool http_conn::read()
{
    if( m_read_idx >= READ_BUFFER_SIZE )
    {
        return false;
    }

    int bytes_read = 0;
    while( true )
    {
        bytes_read = recv( m_socketfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0 );
        //printf("read...bytes_read: %d\n", bytes_read);
        if ( bytes_read == -1 )
        {
            if( errno == EAGAIN || errno == EWOULDBLOCK )
            {
                break;
            }
            return false;
        }
        else if ( bytes_read == 0 )
        {
            return false;
        }

        m_read_idx += bytes_read;
    }
    return true;
}

//解析请求行(request-line)
//[Method]+[空格]+[URL]+[空格]+[Version]+[\r\n]
http_conn::HTTP_CODE http_conn::parse_request_line( char* text )
{
    //printf("parse_request_line input.text===%s\n", text);
    m_url = strpbrk( text, " \t" );
    if ( ! m_url )
    {
        return BAD_REQUEST;
    }
    *m_url++ = '\0';

    //Method
    char* method = text;
    if ( strcasecmp( method, "GET" ) == 0 )
    {
        m_method = GET;
    }
    else if ( strcasecmp( method, "POST" ) == 0 )
    {
        m_method = POST;
    }
    else
    {
        return BAD_REQUEST;
    }

    m_url += strspn( m_url, " \t" );
    m_version = strpbrk( m_url, " \t" );
    if ( ! m_version )
    {
        return BAD_REQUEST;
    }
    *m_version++ = '\0';
    m_version += strspn( m_version, " \t" );
    if ( strcasecmp( m_version, "HTTP/1.1" ) != 0 )
    {
        return BAD_REQUEST;
    }
    if ( strncasecmp( m_url, "http://", 7 ) == 0 )
    {
        m_url += 7;
        m_url = strchr( m_url, '/' );
    }
    if ( ! m_url || m_url[ 0 ] != '/' )
    {
        return BAD_REQUEST;
    }

    //printf("parse_request_line.m_url======%s \n", m_url);
    m_check_state = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::parse_headers( char* text )
{
    //printf("parse_headers===input text:%s \n", text);
    if( text[0] == '\0' ) //请求头结束
    {
        if ( m_method == GET || m_method == HEAD )
        {
            return GET_REQUEST;
        }
        else if ( m_method == POST && m_content_length_request != 0 )
        {
            m_check_state = CHECK_STATE_REQUESTBODY;
            return NO_REQUEST;
        }

        return GET_REQUEST;
    }
    else if ( strncasecmp( text, "User-Agent:", 11 ) == 0 )
    {
        text += 11;
        text += strspn( text, " \t" );
        m_user_agent = text;
    }
    else if ( strncasecmp( text, "Connection:", 11 ) == 0 )
    {
        text += 11;
        text += strspn( text, " \t" );
        if ( strcasecmp( text, "keep-alive" ) == 0 )
        {
            m_linger = true;
        }
    }
    else if ( strncasecmp( text, "Content-Type:", 13 ) == 0 )
    {
        text += 13;
        text += strspn( text, " \t" );
        m_content_type_request = text;
    }
    else if ( strncasecmp( text, "Content-Length:", 15 ) == 0 )
    {
        text += 15;
        text += strspn( text, " \t" );
        m_content_length_request = atol( text );
    }
    else if ( strncasecmp( text, "Host:", 5 ) == 0 )
    {
        text += 5;
        text += strspn( text, " \t" );
        m_host = text;
    }
    else if ( strncasecmp( text, "Accept:", 7 ) == 0 )
    {
        text += 7;
        text += strspn( text, " \t" );
        m_accept = text;
    }
    else
    {
        printf( "parse headers failed! unknow header %s\n", text );
    }

    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::parse_request_body( char* text )
{
    if ( m_read_idx >= ( m_content_length_request + m_checked_idx ) )
    {
        text[ m_content_length_request ] = '\0';
        m_request_body = text;
        return GET_REQUEST;
    }

    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::process_read()
{
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char* text = 0;

    while ( ( ( m_check_state == CHECK_STATE_REQUESTBODY ) && ( line_status == LINE_OK  ) )
                || ( ( line_status = parse_line() ) == LINE_OK ) )
    {
        text = get_line();
        m_start_line = m_checked_idx;
        //printf( "received http line: %s\n", text );
        //printf("m_check_state====%d\n", m_check_state);
        switch ( m_check_state )
        {
            case CHECK_STATE_REQUESTLINE:
            {
                ret = parse_request_line( text );
                if ( ret == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:
            {
                ret = parse_headers( text );
                //printf("parse_headers.ret=%d\n", ret);
                if ( ret == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                else if ( ret == GET_REQUEST )
                {
                    return do_request();
                }
                break;
            }
            case CHECK_STATE_REQUESTBODY:
            {
                ret = parse_request_body( text );
                if ( ret == GET_REQUEST )
                {
                    return do_request();
                }
                line_status = LINE_OPEN;
                break;
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }

    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::do_request()
{
    //判断请求类型
    if (strncasecmp( m_url, "/function", 9 ) == 0)
    {
        m_function = m_url + 10;
        http_function hf(m_function, m_request_body);
        if ( hf.FunctionProcess(m_response_body) ){
            //printf("do_request.m_response_body=====%s\n", m_response_body);
            return FUNCTION_REQUEST;
        }
        else{
            printf("do_request.FunctionProcess error\n");
            return BAD_REQUEST;
        }
    }
    else if (strncasecmp( m_url, "/file", 5 ) == 0)
    {
        printf("FILE_REQUEST...m_url: %s\n", m_url);
        strcpy( m_real_file, file_root_path );
        int len = strlen( file_root_path );
        strncpy( m_real_file + len, m_url+6, FILENAME_LEN - len - 1 );
        printf("m_real_file========%s\n", m_real_file);
        if ( stat( m_real_file, &m_file_stat ) < 0 )
        {
            return NO_RESOURCE;
        }
        if ( !( m_file_stat.st_mode & S_IROTH ) )
        {
            return FORBIDDEN_REQUEST;
        }
        if ( S_ISDIR( m_file_stat.st_mode ) )
        {
            return BAD_REQUEST;
        }
        printf("m_real_file========%s\n", m_real_file);
        int fd = open( m_real_file, O_RDONLY );
        m_file_address = ( char* )mmap( 0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0 );
        printf("do_request.FILE_REQUEST.m_file_address===%s\n", m_file_address);
        close( fd );
        return FILE_REQUEST;
    }
    else
        return BAD_REQUEST;
}

void http_conn::unmap()
{
    if( m_file_address )
    {
        munmap( m_file_address, m_file_stat.st_size );
        m_file_address = 0;
    }
}

bool http_conn::write()
{
    //printf("write start\n");
    int temp = 0;
    int bytes_have_send = 0;
    int bytes_to_send = m_write_idx;
    if ( bytes_to_send == 0 )
    {
        //printf("write bytes_to_send == 0\n");
        modfd( m_epollfd, m_socketfd, EPOLLIN );
        init();
        return true;
    }
    //printf("write...bytes_to_send = %d \n", bytes_to_send);
    while( 1 )
    {
        temp = writev( m_socketfd, m_iv, m_iv_count );
        //printf("write...writev temp = %d \n", temp);
        if ( temp <= -1 )
        {
            if( errno == EAGAIN )
            {
                modfd( m_epollfd, m_socketfd, EPOLLOUT );
                return true;
            }
            unmap();
            return false;
        }
        
        bytes_to_send -= temp;
        bytes_have_send += temp;
        if ( bytes_to_send <= bytes_have_send )
        {
            printf("write end\n");
            unmap();
            if( m_linger )
            {
                init();
                modfd( m_epollfd, m_socketfd, EPOLLIN );
                return true;
            }
            else
            {
                modfd( m_epollfd, m_socketfd, EPOLLIN );
                return false;
            } 
        }
    }
}

bool http_conn::add_response( const char* format, ... )
{
    if( m_write_idx >= WRITE_BUFFER_SIZE )
    {
        return false;
    }
    va_list arg_list;
    va_start( arg_list, format );
    int len = vsnprintf( m_write_buf + m_write_idx, WRITE_BUFFER_SIZE - 1 - m_write_idx, format, arg_list );
    if( len >= ( WRITE_BUFFER_SIZE - 1 - m_write_idx ) )
    {
        return false;
    }
    m_write_idx += len;
    va_end( arg_list );
    return true;
}


//返回响应：状态行(status-line)
//[Version]+[空格]+[Status]+[空格]+[Description]+[\r\n]
bool http_conn::add_status_line( int status, const char* title )
{
    return add_response( "%s %d %s\r\n", "HTTP/1.1", status, title );
}

bool http_conn::add_headers( int content_length, const char *content_type )
{
    if( content_type ) add_content_type( content_type );
    if( content_length > 0 ) add_content_length( content_length );
    add_linger();
    add_blank_line();
}

//Content-Type: application/timestamp-reply
bool http_conn::add_content_type( const char *content_type )
{
    return add_response( "Content-Type: application/%s\r\n", content_type);
}

bool http_conn::add_content_length( int content_length )
{
    return add_response( "Content-Length: %d\r\n", content_length );
}

bool http_conn::add_linger()
{
    return add_response( "Connection: %s\r\n", ( m_linger == true ) ? "keep-alive" : "close" );
}

bool http_conn::add_blank_line()
{
    return add_response( "%s", "\r\n" );
}

bool http_conn::add_body( const char* content )
{
    return add_response( "%s", content );
}

bool http_conn::process_write( HTTP_CODE ret )
{
    //printf("process_write start\n");
    switch ( ret )
    {
        case INTERNAL_ERROR:
        {
            add_status_line( 500, error_500_title );
            add_headers( strlen( error_500_form ), NULL );
            if ( ! add_body( error_500_form ) )
            {
                return false;
            }
            break;
        }
        case BAD_REQUEST:
        {
            add_status_line( 400, error_400_title );
            add_headers( strlen( error_400_form ), NULL );
            if ( ! add_body( error_400_form ) )
            {
                return false;
            }
            break;
        }
        case NO_RESOURCE:
        {
            add_status_line( 404, error_404_title );
            add_headers( strlen( error_404_form ), NULL );
            if ( ! add_body( error_404_form ) )
            {
                return false;
            }
            break;
        }
        case FORBIDDEN_REQUEST:
        {
            add_status_line( 403, error_403_title );
            add_headers( strlen( error_403_form ), NULL );
            if ( ! add_body( error_403_form ) )
            {
                return false;
            }
            break;
        }
        case FUNCTION_REQUEST:
        {
            if (strcasecmp(m_content_type_request, "application/timestamp-query") == 0){
                add_status_line( 200, ok_200_title );
                add_headers( strlen(m_response_body), "timestamp-reply" );
                add_body( m_response_body );
                /*
                encapsulation_write_data();
                printf("FUNCTION_REQUEST end\n");
                return true;*/
            } else {
                add_status_line( 400, error_400_title );
                add_headers( strlen( error_400_form ), NULL );
                if ( ! add_body( error_400_form ) )
                {
                    return false;
                }
            }
            break;
        }
        case FILE_REQUEST:
        {
            add_status_line( 200, ok_200_title );
            if ( m_file_stat.st_size != 0 )
            {
                add_headers( m_file_stat.st_size, NULL );
                m_iv[ 0 ].iov_base = m_write_buf;
                m_iv[ 0 ].iov_len = m_write_idx;
                m_iv[ 1 ].iov_base = m_file_address;
                m_iv[ 1 ].iov_len = m_file_stat.st_size;
                m_iv_count = 2;
                return true;
            }
            else
            {
                const char* ok_string = "<html><body></body></html>";
                add_headers( strlen( ok_string ), NULL );
                if ( ! add_body( ok_string ) )
                {
                    return false;
                }
            }
        }
        default:
        {
            return false;
        }
    }

    //printf("process_write end\n");
    //printf("process_write======m_write_buf :\n %s\n", m_write_buf);
    //printf("process_write======m_write_idx = %d\n", m_write_idx);
    m_iv[ 0 ].iov_base = m_write_buf;
    m_iv[ 0 ].iov_len = m_write_idx;
    m_iv_count = 1;
    return true;
}
/*
void http_conn::encapsulation_write_data()
{
    printf("encapsulation_write_data start\n");
    m_iv_count = (m_write_idx / WRITE_BUFFER_SINGLE_SIZE) + 1;
    printf("encapsulation_write_data..m_iv_count: %d\n", m_iv_count);
    int offset = 0;
    for(int i=0; i < m_iv_count; i++)
    {
        memset(m_write_single_buf[i], 0, sizeof(m_write_single_buf[i]));
        strncpy(m_write_single_buf[i], m_write_buf+offset, WRITE_BUFFER_SINGLE_SIZE);

        m_iv[i].iov_base = m_write_single_buf[i];
        m_iv[i].iov_len = strlen(m_write_single_buf[i]);
        //printf("encapsulation_write_data..m_iv[%d].iov_base: %s\n", i, m_write_single_buf[i]);
        //printf("encapsulation_write_data..m_iv[%d].iov_len: %s\n", i, WRITE_BUFFER_SINGLE_SIZE);
        offset += WRITE_BUFFER_SINGLE_SIZE;
    }

    printf("encapsulation_write_data end\n");
}
*/
void http_conn::process()
{
    //printf("pthread_self======%lu\n", pthread_self());
    HTTP_CODE read_ret = process_read();
    //printf("process_read======read_ret:%d\n", read_ret);
    if ( read_ret == NO_REQUEST )
    {
        modfd( m_epollfd, m_socketfd, EPOLLIN );
        return;
    }

    bool write_ret = process_write( read_ret );
    //printf("process_write======write_ret:%d\n", write_ret);
    if ( ! write_ret )
    {
        close_conn();
    }

    modfd( m_epollfd, m_socketfd, EPOLLOUT );
}

