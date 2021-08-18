#ifndef __KS_UTILFUNC_H__
#define __KS_UTILFUNC_H__     

int ks_socket_connect(const char *pszServerIP, int nPort);
int ks_socket_connect_ds(const char *server_addr_path, char *client_addr_path);
void ks_socket_close(int sock, char *client_addr_path);
int ks_socket_recv(int sock, unsigned char *pBuff, int nSize);
int ks_socket_recvn(int sock, unsigned char *pBuff, int nSize);
int ks_socket_sendn(int sock, unsigned char *pBuff, int nSize);

unsigned long ks_get_tick_count();
unsigned long ks_get_tick_count_us();
char *ks_random_uuid(char *buf);

typedef void (*sigev_notify_func)(union sigval);
bool ks_start_timer(int seconds, sigev_notify_func func, void *param, timer_t *ptid);
bool ks_kill_timer(timer_t tid);

bool ks_read_file(const char *pszFilePath, KTByteArray& rData);
bool ks_write_file(const char *pszFilePath, unsigned char *pbData, int nDataLen);

int ks_get_pid_by_name(const char *pszProcessName);

#endif  // __KS_UTILFUNC_H__
