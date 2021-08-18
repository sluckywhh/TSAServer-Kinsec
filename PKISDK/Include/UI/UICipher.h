#ifndef __UI_CIPHER_H__
#define __UI_CIPHER_H__

/**
  * @Name: UICipher.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/30 15:47
  * @Description: UICipher module interface file
*/

#include "UIDef.h"

#ifdef __cplusplus
extern "C"	{
#endif	

////error info begin
KT_LIB_MODE int KTCore_GetLastErrNum();
///error info end 

KT_LIB_MODE int KTCore_Thread_Initialize();
KT_LIB_MODE int KTCore_Thread_Finalize();
						  
KT_LIB_MODE const KTChar* KTCore_GetDecryptedFileNames(
						IN OUT int* pnLen,
						OUT char* pc_fileName );

	//encrypt and decrypt with symmetric key 
KT_LIB_MODE const KTChar* KTCore_Encrypt( LPCTSTR szPlainFiles,  
						LPCTSTR szCipherFile,
						bool bCertBakPwd,
						int unPwdLen,
						BYTE* pbPwd );   //可为NULL

	//only decrypt with symmetric key
KT_LIB_MODE const KTChar* KTCore_Decrypt( LPCTSTR szCipherFile , 
					    LPCTSTR szPlainFileOrFolder, 
						bool bUseBakCert, 
						int unPwdLen, 
						BYTE* pbPwd,		//可为NULL
						BOOL bOpenPlainFiles );  

	//decrypt with symmetiric key and develope use session
KT_LIB_MODE const KTChar* KTCore_DecryptOrDevelop( LPCTSTR szCipherFile, 
					    LPCTSTR szPlainFileOrFolder,
						bool bUseBakCert, 
						BOOL bOpenPlainFiles );

	////envelop  according  certificate handle or addresser emails /
KT_LIB_MODE const KTChar* KTCore_Envelop( LPCTSTR szPlainFiles, 
						LPCTSTR szCipherFile,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE* phHandles	,
						LPCTSTR szAddressee );		//可为NULL, 只在phHandles为NULL时起作用

	////only develop in session
KT_LIB_MODE const KTChar* KTCore_Develop(LPCTSTR szCipherFile, 
					    LPCTSTR szPlainFileOrFolder,	//if folder end with \, please
						KT_SESSION_HANDLE hSession,
						BOOL bOpenPlainFiles );

	/////update addresser envelopes
KT_LIB_MODE const KTChar* KTCore_UpdateEnvelop( LPCTSTR szCipherFile,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE* phHandles	,
						LPCTSTR szAddressee );		//可为NULL, 只在phHandles为NULL时起作用

	////sign awith certificate handle
KT_LIB_MODE const KTChar* KTCore_Sign( LPCTSTR szPlainFile, 
						LPCTSTR  szSignatureFile, 
						BOOL bPlainInSignature,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE* phHandles	,						
						LPCTSTR szAddresser );		//可为NULL, 只在phHandles为NULL时起作用

KT_LIB_MODE const KTChar* KTCore_Sign_SoftCert( LPCTSTR szPlainFile, 
						LPCTSTR  szSignatureFile, 
						BOOL bPlainInSignature,
						KTCertificate *pSoftCert,
						CKey *pSoftKey,
						LPCTSTR szAddresser );

	////fisrt envelop then sign 
KT_LIB_MODE const KTChar* KTCore_EnvelopSign( LPCTSTR szPlainFiles, 
						    LPCTSTR szCipherSignatureFile, 
							KT_SESSION_HANDLE hSession,		//可为NULL
							int	nEnveHandleCount,
							KTHANDLE* phEnveHandles	,
							int	nSignHandleCount,
							KTHANDLE* phSignHandles	,
							LPCTSTR szAddresser,			//可为NULL,只在phSignHandles为NULL时起作用
							LPCTSTR szAddressee );			//可为NULL,只在phEnveHandles为NULL时起作用

	////update addresser envelopes and addressee signature 
KT_LIB_MODE const KTChar* KTCore_UpdateEnvelopSign( LPCTSTR szCipherSignatureFile, 
							KT_SESSION_HANDLE hSession,		//可为NULL
							int	nEnveHandleCount,
							KTHANDLE* phEnveHandles	,
							int	nSignHandleCount,
							KTHANDLE* phSignHandles	,
							LPCTSTR szAddresser,			//可为NULL,只在phSignHandles为NULL时起作用
							LPCTSTR szAddressee );			//可为NULL,只在phEnveHandles为NULL时起作用

	///first verify then develope if need	
KT_LIB_MODE const KTChar* KTCore_VerifySign( LPCTSTR szSignatureFile,
							LPCTSTR szPlainFileOrFolder,  
							KT_SESSION_HANDLE hSession,
							BOOL bOpenPlainFiles, 
							KT_SIGN_INFO* pSignInfo );	 //可为NULL

/////////////////////////////////////////
/////safe delete file begin
KT_LIB_MODE const KTChar* KTCore_DeleteFilesFolders( LPCTSTR szFilesOrFolders,
					  int nWriteRandomTimes,
					  BOOL bDeleteCofferFile,
                      BOOL bSilent );
/////safe delete file end
/////////////////////////////////////////


/////show signature info
KT_LIB_MODE const KTChar* KTCore_ShowSignature( KT_SESSION_HANDLE hSession, 
						 KT_SIGN_INFO* pSignInfo );


//////Mail function begin
KT_LIB_MODE const KTChar* KTCore_MailGenB64RandomAndTag(	unsigned int unCharLen, //suggest 4n+1
				  char pc_char[],
				  BOOL bEnvelop,  
				  BOOL bSign, 
				  unsigned int unLineIndex,//Line index,ex 96
				  unsigned int unMailType ); //0, NotesEx; 1, MailProxy

KT_LIB_MODE const KTChar* KTCore_MailMsgAddTag( unsigned int* punCipherMsgLen,		
					char pc_cipherTagMsg[],
					LPCTSTR pszMessage,
					BOOL bSign, BOOL bEncrypt );

KT_LIB_MODE const KTChar* KTCore_MailDecodeB64( const char pc_b64[],
						 BOOL * pbIsEncrypt,
						 BOOL * pbIsSign,
						 unsigned int* punMailType, //0, NotesEx; 1, MailProxy; 2, FileEx
						 char pc_tmpNdsFileNm[MAX_PATH] );
//////Mail function end

KT_LIB_MODE const KTChar*  KTCore_OpenPlainFiles( LPCTSTR szPlainFileOrFolder, BOOL bDelete, int nDeleteTime );
//Check TimeStamp Server certificate.
KT_LIB_MODE BOOL KTCore_IsTrustedTssCert(KT_SESSION_HANDLE hSession,KTCertificate &cert);

#ifdef __cplusplus

//Hongwei Yu 2003/11/12 added for secSeal

	////envelop  according  certificate handle or addresser emails /
KT_LIB_MODE const KTChar* KTCore_EnvelopEx( CFile& fPlainFile, 
						CFile& fCipherFile,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE* phHandles	,
						LPCTSTR szAddressee );		//可为NULL, 只在phHandles为NULL时起作用 
						
	////only develop in session
KT_LIB_MODE const KTChar* KTCore_DevelopEx( CFile& fCipherFile, 
					    CFile& fPlainFile,
						KT_SESSION_HANDLE hSession );

	////sign with certificate handle
KT_LIB_MODE const KTChar* KTCore_SignEx( CFile& fPlainFile, 
						CFile& fSignatureFile, 
						BOOL bPlainInSignature,
						KT_SESSION_HANDLE hSession,	//可为NULL
						int	nHandleCount,
						KTHANDLE* phHandles	,						
						LPCTSTR szAddresser );		//可为NULL, 只在phHandles为NULL时起作用

KT_LIB_MODE const KTChar* KTCore_SignEx_SoftCert( CFile& fPlainFile, 
						CFile& fSignatureFile, 
						BOOL bPlainInSignature,
						KTCertificate *pSoftCert,
						CKey *pSoftKey,
						LPCTSTR szAddresser );

	////fisrt envelop then sign 
KT_LIB_MODE const KTChar* KTCore_EnvelopSignEx( CFile& fPlainFiles, 
						    CFile& fCipherSignatureFile, 
							KT_SESSION_HANDLE hSession,		//可为NULL
							int	nEnveHandleCount,
							KTHANDLE* phEnveHandles	,
							int	nSignHandleCount,
							KTHANDLE* phSignHandles	,
							LPCTSTR szAddresser,			//可为NULL,只在phSignHandles为NULL时起作用
							LPCTSTR szAddressee );			//可为NULL,只在phEnveHandles为NULL时起作用

	///first verify then develope if need	
KT_LIB_MODE const KTChar* KTCore_VerifySignEx( CFile& fSignatureFile,
							CFile& fPlainFileOrFolder,  
							KT_SESSION_HANDLE hSession,
							KT_SIGN_INFO* pSignInfo );	 //可为NULL


//Hongwei Yu 2003/11/12 added for secSeal

KT_LIB_MODE const KTChar* KTCore_MailB64CodeFile( const char pc_inputFileNm[],
						    BOOL bAddEncryptTag, BOOL bAddSignTag,
						    KTString& strB64 );

class KTTimeSet;
class KTCertificates;
class KTRecipientInfos;
class KTSignerInfoSequence;
KT_LIB_MODE const KTChar* KTCore_GetEnvelopFileRecipientInfo(const char pc_cipherFile[],
															KTRecipientInfos& recipientInfo,
															KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar* KTCore_GetEnvelopFileRecipientInfoEx(CFile& fCipherFile,
															KTRecipientInfos& recipientInfo,
															KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar* KTCore_GetEnvelopSignFileRecipientInfo(const char pc_signFile[],
															KTRecipientInfos& recipientInfo,
															KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar* KTCore_GetEnvelopSignFileRecipientInfoEx(CFile& fSignFile,
															KTRecipientInfos& recipientInfo,
															KTCertificates& recipientCerts);

KT_LIB_MODE const KTChar* KTCore_GetSignedFileSignInfo(const char pc_signFile[],
													   KTTimeSet& signTimeSet,
													   KTSignerInfoSequence& signerInfoSequence,
													   KTCertificates& signCerts);

KT_LIB_MODE const KTChar* KTCore_GetSignedFileSignInfoEx(CFile& fSignFile,
													   KTTimeSet& signTimeSet,
													   KTSignerInfoSequence& signerInfoSequence,
													   KTCertificates& signCerts);
#endif

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

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/30 16:00  Hongwei Yu   Created
///   1.0.0.1  2003/05/14 11:12  Hongwei Yu   modified KTCore_EnvelopSign and KTCore_VerifySign 
///   1.0.0.2  2003/10/15 08:48  Hongwei Yu   added KTCore_GetEnvelop***

#endif 