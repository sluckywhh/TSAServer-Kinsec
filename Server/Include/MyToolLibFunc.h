#ifndef _MYTOOLLIBFUNC_H
#define _MYTOOLLIBFUNC_H

#include "FJCAApiConst.h"

#undef TRUE
#undef FALSE
#define TRUE				0x00000001	//布尔值为真
#define FALSE				0x00000000	//布尔值为假

#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef KTBOOL32
typedef int					KTBOOL32;
#endif
#ifndef WORD
typedef unsigned short int	WORD;
#endif
#ifndef DWORD
#	ifdef WIN32
        typedef unsigned long   DWORD;
#	else
        typedef unsigned int    DWORD;
#	endif
#endif
#ifndef HANDLE
typedef void *				HANDLE;
#endif

#ifdef _WIN32
#include <winsock2.h>
#else
typedef int					SOCKET; 
#endif

// 1
long MYTOOL_Initialize();
long MYTOOL_Finalize();
long MYTOOL_GetReturnString(long lRet, char *pszReturnString);

// 2 get cert info
long MYTOOL_GetCertVersion(unsigned char *cert, long certlen, char *version, long *versionlen);
long MYTOOL_GetCertSerialNumber(unsigned char *cert, long certlen, char *sn, long *snlen);
long MYTOOL_GetCertSerialNumber2(unsigned char *cert, long certlen, char *sn, long *snlen);
long MYTOOL_GetCertSignAlg(unsigned char *cert, long certlen, char *signalg, long *signalglen);
long MYTOOL_GetCertValidity(unsigned char *cert, long certlen, char *notbefore, char *notafter);
long MYTOOL_GetCertDN(unsigned char *cert, long certlen, char *dn, long *dnlen);
long MYTOOL_GetCertIssuerDN(unsigned char *cert, long certlen, char *issuerdn, long *issuerdnlen);
long MYTOOL_GetCertDNItemValue(unsigned char *cert, long certlen, char *itemname, char *itemvalue, long *itemvaluelen);
long MYTOOL_GetCertIssuerDNItemValue(unsigned char *cert, long certlen, char *itemname, char *itemvalue, long *itemvaluelen);
long MYTOOL_GetCertEmail(unsigned char *cert, long certlen, char *email, long *emaillen);
long MYTOOL_GetCertPubKey(unsigned char *cert, long certlen, char *pubkey, long *pubkeylen);
long MYTOOL_GetCertExtensions(unsigned char *cert, long certlen, char *out, long *outlen);

// 3 get cert ext info
long MYTOOL_GetExtAuthorityKeyIdentifier(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtSubjectKeyIdentifier(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtKeyUsage(unsigned char *cert, long certlen, DWORD *keyusage, char *out, long *outlen);
long MYTOOL_GetExtCertificatePolicies(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtPolicyMappings(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtBasicConstraints(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtPolicyConstraints(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtExtKeyUsage(unsigned char *cert, long certlen, DWORD *extkeyusage, char *out, long *outlen);
long MYTOOL_GetExtCrlDistributionPoints(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtNetScapeCertType(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtAuthorityInfoAccess(unsigned char *cert, long certlen, char *out, long *outlen);
long MYTOOL_GetExtByOid(unsigned char *cert, long certlen, char *oid, char *out, long *outlen);

// 4 operation
long MYTOOL_HashData(unsigned char *in, long inlen, unsigned char *out, long *outlen, long hashmethod = SGD_SHA1);
long MYTOOL_HashFile(char *infile, unsigned char *out, long *outlen, long hashmethod = SGD_SHA1);
long MYTOOL_FormDigestInfo(unsigned char *in, long inlen, unsigned char *out, long *outlen, long hashmethod = SGD_SHA1);
long MYTOOL_CompareDigest(unsigned char *in, long inlen, unsigned char *digestinfo, long digestinfolen, KTBOOL32 data = TRUE);

long MYTOOL_EncryptData(unsigned char *key, unsigned char *in, long inlen, unsigned char *out, long *outlen, long encryptmethod = SGD_SM4_ECB);
long MYTOOL_DecryptData(unsigned char *key, unsigned char *in, long inlen, unsigned char *out, long *outlen, long encryptmethod = SGD_SM4_ECB);
long MYTOOL_EncryptFile(unsigned char *key, char *infile, char *outfile, long encryptmethod = SGD_SM4_ECB);
long MYTOOL_DecryptFile(unsigned char *key, char *infile, char *outfile, long encryptmethod = SGD_SM4_ECB);

KTBOOL32 MYTOOL_IsECCCert(unsigned char *cert, long certlen);
long MYTOOL_ECCGetPubKeyFromCert(unsigned char *cert, long certlen, unsigned char *pubkey_x, long *pubkeylen_x, unsigned char *pubkey_y, long *pubkeylen_y);

// 预处理1
// Z = SM3(ENTL‖ID‖a‖b‖Gx‖Gy‖Px‖Py)
// 256bits
long MYTOOL_ECCGetZ(unsigned char *userid, long useridlen, unsigned char *pubkey_x, unsigned char *pubkey_y, unsigned char *out, long *outlen, long ecclen = 32);
// 预处理2
// H = SM3(Z‖M)
long MYTOOL_ECCGetH(unsigned char *z, long zlen, unsigned char *in, long inlen, unsigned char *out, long *outlen);
// 预处理2
// H = SM3(Z‖M)
long MYTOOL_ECCGetFileH(unsigned char *z, long zlen, char *infile, unsigned char *out, long *outlen);

long MYTOOL_YCL(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 data = TRUE);

long MYTOOL_RSAEncryptWithKey(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *key, long keylen, KTBOOL32 encrypt = TRUE);
long MYTOOL_ECCEncryptWithKey(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *key, long keylen, KTBOOL32 encrypt = TRUE);

long MYTOOL_RSAEncryptWithCert(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 encrypt = TRUE);
long MYTOOL_ECCEncryptWithCert(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 encrypt = TRUE);
long MYTOOL_EncryptWithCert(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 encrypt = TRUE);

long MYTOOL_RSAVerifySignedData(unsigned char *in, long inlen, unsigned char *sign, long signlen, unsigned char *cert, long certlen, KTBOOL32 data = TRUE);
long MYTOOL_ECCVerifySignedData(unsigned char *in, long inlen, unsigned char *sign, long signlen, unsigned char *cert, long certlen, KTBOOL32 data = TRUE);
long MYTOOL_VerifySignedData(unsigned char *in, long inlen, unsigned char *sign, long signlen, unsigned char *cert, long certlen, KTBOOL32 data = TRUE);

long MYTOOL_SM2PublicKeyEncode(unsigned char *in_x, long inlen_x, unsigned char *in_y, long inlen_y, unsigned char *out, long *outlen);
long MYTOOL_SM2PublicKeyDecode(unsigned char *in, long inlen, unsigned char *out_x, long *outlen_x, unsigned char *out_y, long *outlen_y);
long MYTOOL_SM2PrivateKeyEncode(unsigned char *in, long inlen, unsigned char *out, long *outlen);
long MYTOOL_SM2PrivateKeyDecode(unsigned char *in, long inlen, unsigned char *out, long *outlen);
long MYTOOL_SM2SignDataEncode(unsigned char *in_r, long inlen_r, unsigned char *in_s, long inlen_s, unsigned char *out, long *outlen);
long MYTOOL_SM2SignDataDecode(unsigned char *in, long inlen, unsigned char *out_r, long *outlen_r, unsigned char *out_s, long *outlen_s);
long MYTOOL_SM2CipherDataEncode(unsigned char *in_c1, long inlen_c1, unsigned char *in_c2, long inlen_c2, unsigned char *in_c3, long inlen_c3, unsigned char *out, long *outlen);
long MYTOOL_SM2CipherDataDecode(unsigned char *in, long inlen, unsigned char *out_c1, long *outlen_c1, unsigned char *out_c2, long *outlen_c2, unsigned char *out_c3, long *outlen_c3);

// 5 communication
SOCKET MYTOOL_SocketConnect(char *pszServerIP, int nPort);
int MYTOOL_SocketSend(SOCKET sock, const void *pBuff, int nSize);
int MYTOOL_SocketSend_n(SOCKET sock, const void *pBuff, int nSize);
int MYTOOL_SocketRecv(SOCKET sock, void *pBuff, int nSize);
int MYTOOL_SocketRecv_n(SOCKET sock, void *pBuff, int nSize);

// 6 verify cert
long MYTOOL_VerifyCertSign(unsigned char *cert, int certlen, unsigned char *cacert, int cacertlen);
long MYTOOL_VerifyCertSign(unsigned char *cert, int certlen, char *cafile);
long MYTOOL_VerifyCertValidity(unsigned char *cert, int certlen);
long MYTOOL_VerifyCRLSign(unsigned char *crl, int crllen, unsigned char *cacert, int cacertlen);
long MYTOOL_VerifyCRLSign(char *crlfile, char *cafile);
long MYTOOL_VerifyCertWithCRL(unsigned char *cert, int certlen, unsigned char *crl, int crllen);
long MYTOOL_VerifyCertWithCRL(unsigned char *cert, int certlen, char *crlfile);
long MYTOOL_VerifyCert(unsigned char *cert, int certlen, unsigned char *cacert, int cacertlen, unsigned char *crl, int crllen, KTBOOL32 bCheckCRL);
long MYTOOL_VerifyCert(unsigned char *cert, int certlen, char *cafile, char *crlfile, KTBOOL32 bCheckCRL);

// 7 others
KTBOOL32 MYTOOL_PKCS5Padding(unsigned char *data, long & datalen, KTBOOL32 encrypt = TRUE);

#endif
