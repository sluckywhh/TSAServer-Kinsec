#ifndef __UI_CIPHER_H__
#define __UI_CIPHER_H__

#ifndef MAX_PATH
#define MAX_PATH					260
#endif

#ifndef KS_PATH_SERAPATOR_CHAR
#define KS_PATH_SERAPATOR_CHAR		'/'
#endif

#ifndef LPCTSTR
typedef const char *		LPCTSTR;
#endif

#include "UIDef.h"

#ifdef __cplusplus
extern "C"	{
#endif

class KTFile
{
public:
	char m_szFilePath[512];
	char m_szFileName[512];
	FILE *m_fp;
	
	KTFile()
	{
		memset(m_szFilePath, 0, sizeof(m_szFilePath));
		memset(m_szFileName, 0, sizeof(m_szFileName));
		m_fp = NULL;
	}

	~KTFile()
	{
		if (m_fp != NULL)
		{
			fclose(m_fp);
			m_fp = NULL;
		}
	}

	KTBOOL32 Open(const char *pszFilePath, const char *pszFlag)
	{
		memset(m_szFilePath, 0, sizeof(m_szFilePath));
		memset(m_szFileName, 0, sizeof(m_szFileName));
		if (m_fp != NULL)
		{
			fclose(m_fp);
			m_fp = NULL;
		}

		m_fp = fopen(pszFilePath, pszFlag);
		if (m_fp == NULL)
			return FALSE;

		strcpy(m_szFilePath, pszFilePath);
		char *pszTmp = strrchr(m_szFilePath, KS_PATH_SERAPATOR_CHAR);
		if (pszTmp != NULL)
			strcpy(m_szFileName, pszTmp + 1);
		else
			strcpy(m_szFileName, m_szFilePath);

		return TRUE;
	}

	void Close()
	{
		if (m_fp != NULL)
		{
			fclose(m_fp);
			m_fp = NULL;
		}
	}

	int GetLength()
	{
		int nCurPos = ftell(m_fp);
		fseek(m_fp, 0, SEEK_END);
		int nLen = ftell(m_fp);
		fseek(m_fp, nCurPos, SEEK_SET);
		return nLen;
	}

	int GetPosition()
	{
		int nCurPos = ftell(m_fp);
		return nCurPos;
	}

	int Read(void *pDta, int nDataLen)
	{
		int nRead = fread(pDta, 1, nDataLen, m_fp);
		return nRead;
	}

	int Write(void *pDta, int nDataLen)
	{
		int nWritten = fwrite(pDta, 1, nDataLen, m_fp);
		return nWritten;
	}

	KTBOOL32 Seek(int nOffset, int nOrigin)
	{
		if (fseek(m_fp, nOffset, nOrigin) != 0)
			return FALSE;

		return TRUE;
	}
};

/////////////////////////////////////////
////error info begin
KT_LIB_MODE int KTCore_GetLastErrNum();
///error info end 

KT_LIB_MODE int KTCore_Thread_Initialize();
KT_LIB_MODE int KTCore_Thread_Finalize();

/////////////////////////////////////////
// ktcore operation
	////sign with certificate handle
KT_LIB_MODE const KTChar *KTCore_Sign(LPCTSTR szPlainFile,
						LPCTSTR szSignatureFile,
						KTBOOL32 bPlainInSignature,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE *phHandles,
						LPCTSTR szAddresser);		//可为NULL, 只在phHandles为NULL时起作用

KT_LIB_MODE const KTChar *KTCore_Sign_SoftCert(LPCTSTR szPlainFile,
						LPCTSTR szSignatureFile,
						KTBOOL32 bPlainInSignature,
						KTCertificate *pSoftCert,
						CKey *pSoftKey,
						LPCTSTR szAddresser);

	////sign with certificate handle
KT_LIB_MODE const KTChar *KTCore_SignEx(KTFile& fPlainFile,
						KTFile& fSignatureFile,
						KTBOOL32 bPlainInSignature,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE *phHandles,
						LPCTSTR szAddresser);		//可为NULL, 只在phHandles为NULL时起作用

KT_LIB_MODE const KTChar *KTCore_SignEx_SoftCert(KTFile& fPlainFile,
						KTFile& fSignatureFile,
						KTBOOL32 bPlainInSignature,
						KTCertificate *pSoftCert,
						CKey *pSoftKey,
						LPCTSTR szAddresser);

	///first verify then develope if need	
KT_LIB_MODE const KTChar *KTCore_VerifySign(LPCTSTR szSignatureFile,
						LPCTSTR szPlainFileOrFolder,
						KT_SESSION_HANDLE hSession,
						KTBOOL32 bOpenPlainFiles,
						KT_SIGN_INFO *pSignInfo);	 //可为NULL

	///first verify then develope if need	
KT_LIB_MODE const KTChar *KTCore_VerifySignEx(KTFile& fSignatureFile,
						KTFile& fPlainFileOrFolder,
						KT_SESSION_HANDLE hSession,
						KT_SIGN_INFO *pSignInfo);	 //可为NULL

	////envelop  according  certificate handle or addresser emails /
KT_LIB_MODE const KTChar *KTCore_Envelop(LPCTSTR szPlainFiles,
						LPCTSTR szCipherFile,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE *phHandles,
						LPCTSTR szAddressee);		//可为NULL, 只在phHandles为NULL时起作用

	////envelop  according  certificate handle or addresser emails /
KT_LIB_MODE const KTChar *KTCore_EnvelopEx(KTFile& fPlainFile,
						KTFile& fCipherFile,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE *phHandles,
						LPCTSTR szAddressee);		//可为NULL, 只在phHandles为NULL时起作用 

	////only develop in session
KT_LIB_MODE const KTChar *KTCore_Develop(LPCTSTR szCipherFile,
					    LPCTSTR szPlainFileOrFolder,	//if folder end with \, please
						KT_SESSION_HANDLE hSession,
						KTBOOL32 bOpenPlainFiles);
						
	////only develop in session
KT_LIB_MODE const KTChar *KTCore_DevelopEx(KTFile& fCipherFile,
					    KTFile& fPlainFile,
						KT_SESSION_HANDLE hSession);

	////fisrt envelop then sign 
KT_LIB_MODE const KTChar *KTCore_EnvelopSign(LPCTSTR szPlainFiles,
						LPCTSTR szCipherSignatureFile,
						KT_SESSION_HANDLE hSession,		//可为NULL
						int	nEnveHandleCount,
						KTHANDLE *phEnveHandles,
						int	nSignHandleCount,
						KTHANDLE *phSignHandles,
						LPCTSTR szAddresser,			//可为NULL,只在phSignHandles为NULL时起作用
						LPCTSTR szAddressee);			//可为NULL,只在phEnveHandles为NULL时起作用

	////fisrt envelop then sign 
KT_LIB_MODE const KTChar *KTCore_EnvelopSignEx(KTFile& fPlainFiles,
						KTFile& fCipherSignatureFile,
						KT_SESSION_HANDLE hSession,		//可为NULL
						int	nEnveHandleCount,
						KTHANDLE *phEnveHandles,
						int	nSignHandleCount,
						KTHANDLE *phSignHandles,
						LPCTSTR szAddresser,			//可为NULL,只在phSignHandles为NULL时起作用
						LPCTSTR szAddressee);			//可为NULL,只在phEnveHandles为NULL时起作用

	//encrypt and decrypt with symmetric key 
KT_LIB_MODE const KTChar *KTCore_Encrypt(LPCTSTR szPlainFiles,
						LPCTSTR szCipherFile,
						bool bCertBakPwd,
						int unPwdLen,
						BYTE *pbPwd);   //可为NULL

	//only decrypt with symmetric key
KT_LIB_MODE const KTChar *KTCore_Decrypt(LPCTSTR szCipherFile,
					    LPCTSTR szPlainFileOrFolder,
						bool bUseBakCert,
						int unPwdLen,
						BYTE *pbPwd,		//可为NULL
						KTBOOL32 bOpenPlainFiles);

	//decrypt with symmetiric key and develope use session
KT_LIB_MODE const KTChar *KTCore_DecryptOrDevelop(LPCTSTR szCipherFile,
					    LPCTSTR szPlainFileOrFolder,
						bool bUseBakCert,
						KTBOOL32 bOpenPlainFiles);

KT_LIB_MODE const KTChar *KTCore_GetDecryptedFileNames(
						IN OUT int *pnLen,
						OUT char *pc_fileName);

/////////////////////////////////////////
// others
//Check TimeStamp Server certificate.
KT_LIB_MODE KTBOOL32 KTCore_IsTrustedTssCert(KT_SESSION_HANDLE hSession,
						KTCertificate &cert);

/////show signature info
KT_LIB_MODE const KTChar *KTCore_ShowSignature(KT_SESSION_HANDLE hSession,
						KT_SIGN_INFO *pSignInfo);

class KTTimeSet;
class KTCertificates;
class KTRecipientInfos;
class KTSignerInfoSequence;

KT_LIB_MODE const KTChar *KTCore_GetSignedFileSignInfo(const char pc_signFile[],
						KTTimeSet& signTimeSet,
						KTSignerInfoSequence& signerInfoSequence,
						KTCertificates& signCerts);

KT_LIB_MODE const KTChar *KTCore_GetSignedFileSignInfoEx(KTFile& fSignFile,
						KTTimeSet& signTimeSet,
						KTSignerInfoSequence& signerInfoSequence,
						KTCertificates& signCerts);

KT_LIB_MODE const KTChar *KTCore_GetEnvelopFileRecipientInfo(const char pc_cipherFile[],
						KTRecipientInfos& recipientInfo,
						KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar *KTCore_GetEnvelopFileRecipientInfoEx(KTFile& fCipherFile,
						KTRecipientInfos& recipientInfo,
						KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar *KTCore_GetEnvelopSignFileRecipientInfo(const char pc_signFile[],
						KTRecipientInfos& recipientInfo,
						KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar *KTCore_GetEnvelopSignFileRecipientInfoEx(KTFile& fSignFile,
						KTRecipientInfos& recipientInfo,
						KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar *KTCore_SignDataByP7(bool bAttached, unsigned char *pbIn, int nInLen, unsigned char *pbOut, int *pnOutLen);
KT_LIB_MODE const KTChar *KTCore_SignDataByP7Ex(bool bAttached, bool bHasAttr, unsigned char *pbIn, int nInLen, unsigned char *pbOut, int *pnOutLen);
KT_LIB_MODE const KTChar *KTCore_VerifySignedDataByP7(unsigned char *pbP7Sign, int nP7SignLen, unsigned char *pbData, int nDataLen);
// nType：1－原文，2－签名证书，3－签名值，4－签名算法，5－原文摘要，6－摘要算法，7－签名数据版本，8－签名时间，18－签名时间（Z结尾）
KT_LIB_MODE const KTChar *KTCore_GetP7SignDataInfo(unsigned char *pbIn, int nInLen, int nType, unsigned char *pbOut, int *pnOutLen);

// nAlgID：
// 0x00000401－SM4算法ECB加密模式（推荐）
// 0x00000402－SM4算法CBC加密模式
// 其它－3DES
KT_LIB_MODE const KTChar *KTCore_EnvelopeDataByP7(int nAlgID, unsigned char *pbIn, int nInLen, char *pszEncCertsB64, unsigned char *pbOut, int *pnOutLen);
KT_LIB_MODE const KTChar *KTCore_UnenvelopeDataByP7(unsigned char *pbIn, int nInLen, unsigned char *pbOut, int *pnOutLen);
// nType：1－接收者证书的颁发者DN和序列号（DN1;序列号1|DN2;序列号2...）
KT_LIB_MODE const KTChar *KTCore_GetP7EnvelopeDataInfo(unsigned char *pbIn, int nInLen, int nType, unsigned char *pbOut, int *pnOutLen);

KT_LIB_MODE const KTChar *KTCore_SignAndEnvelopeDataByP7(int nAlgID, unsigned char *pbIn, int nInLen, char *pszEncCertsB64, unsigned char *pbOut, int *pnOutLen);
KT_LIB_MODE const KTChar *KTCore_UnenvelopeAndVerifyDataByP7(unsigned char *pbIn, int nInLen, unsigned char *pbOut, int *pnOutLen);
// nType：2－签名证书，3－签名值
KT_LIB_MODE const KTChar *KTCore_GetP7SignAndEnvelopeDataInfo(unsigned char *pbIn, int nInLen, int nType, unsigned char *pbOut, int *pnOutLen);

#ifdef __cplusplus
}
#endif

#endif 