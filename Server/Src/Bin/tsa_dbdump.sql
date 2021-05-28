-- 
-- postgreSQL database dump
--

\connect - root

SET search_path = public, pg_catalog;

--
-- Name: t_time_stamp; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_time_stamp ( 
    f_entry_time timestamp, 
    f_serial_number text, 
    f_data text 
); 

-- 
-- Name: t_event_log; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_event_log ( 
    f_log_time timestamp, 
    f_event_title text, 
    f_event_info text 
); 

-- 
-- Name: t_setting_log; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_setting_log ( 
    f_log_time timestamp, 
    f_setting_user text, 
    f_setting_info text 
); 

-- 
-- Name: t_visit_log; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_visit_log ( 
    f_log_time timestamp, 
    f_client_ip text, 
    f_operater text 
); 

-- 
-- Name: t_request_log; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_request_log ( 
    f_log_time timestamp, 
    f_request_data text, 
    f_response_data text, 
    f_status text 
); 

-- 
-- Name: t_query_log; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_query_log ( 
    f_log_time timestamp, 
    f_query_type text, 
    f_query_condition text, 
    f_query_result text 
); 

-- 
-- Name: t_revoke_log; Type: TABLE; Schema: public; Owner: root
-- 
CREATE TABLE t_revoke_log ( 
    f_log_time timestamp, 
    f_revoke_user text, 
    f_revoke_reason text, 
    f_revoke_sn text, 
    f_revoke_data text 
); 


