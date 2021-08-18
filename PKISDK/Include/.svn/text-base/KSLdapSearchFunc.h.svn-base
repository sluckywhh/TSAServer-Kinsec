#ifndef __KS_LDAPSEARCH__FUNC__
#define __KS_LDAPSEARCH__FUNC__

#ifdef _WIN32
	#ifndef KSLDAPSEARCH_EXPORTS
		#if defined(_DEBUG)
			#pragma comment(lib, "KSLdapSearchd.lib")
			#pragma message("Automatically linking with KSLdapSearchd.lib")
		#else
			#pragma comment(lib, "KSLdapSearch.lib")
			#pragma message("Automatically linking with KSLdapSearch.lib")
		#endif	
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

int ldap_set_cb(void* callback);

// 以人查证
int ldap_get_info(char* host_port, char* userid, char* info, char* path);
// 以证查证
//int ldap_get_cert_info(char* host_port, char* typeId, char* fileId, char* selCode, char* info, char* path);
int ldap_get_cert_info(char* host_port, char* url, char* info, char* path);

// 以人查证，info返回数据多了二维码
int ldap_get_info_qrcode(char* host_port, char* userid, char* info, char* path);
// 根据二维码查证
int ldap_get_cert_info_qrcode(char* url, char* info, char* path);

#ifdef __cplusplus
}
#endif

#endif
