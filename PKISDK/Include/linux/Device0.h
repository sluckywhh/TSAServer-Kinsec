// Device0.h : Declaration of the CDevice0

#ifndef __DEVICE0_H_
#define __DEVICE0_H_

#include "SKFApiFuncDef.h"
#include "DevDefs.h"

/////////////////////////////////////////////////////////////////////////////
// CDevice0
class CDevice0 
{
public:
	CDevice0();
	~CDevice0();

	KTString id;

public:
	int GetDLLType(int *dllType, int *retval);

	int OpenDev(int *retval);
	int OpenDevEx(bool connectDevOnly, int *retval);
	int CloseDev(int *retval);

	int QueryState(DEVICE_STATE *state, int *retval);
	int GetDeviceType(int *type, int *retval);
	int GetDeviceInfo(DEVINFO *info, int *retval);
	int GetDeviceSN(KTCharArray& devSN, int *retval);
	int GetRandom(int dataLen, KTByteArray& randomData, int *retval);

	int FormatDev(KTString adminPin, int adminPinMaxRetryCount, KTString userPin, int userPinMaxRetryCount, int *retval);

	int Login(KTString pin, int *rcount, int keep, int *retval);
	int VerifyUserPin(KTString userPin, int *rcount, int *retval);
	int VerifyAdminPin(KTString adminPin, int *rcount, int *retval);
	int Logout(int *retval);
	int ChangePin(KTString oldPin, KTString newPin, int *rcount, int *retval);
	int ChangeAdminPin(KTString oldPin, KTString newPin, int *rcount, int *retval);
	int UnlockUserPin(KTString adminPin, KTString userPin, int *rcount, int *retval);

	int GetCertNo(int *pCertNo, int *retval);
	int ECCGetCertNo(int *pCertNo, int *retval);
	int ReadCert(int pair, KTByteArray& cert, int *retval);
	int ECCReadCert(int pair, KTByteArray& cert, int *retval);
	int WriteCert(int pair, KTByteArray& cert, int *retval);
	int ECCWriteCert(int pair, KTByteArray& cert, int *retval);

	int PrivateKeyEncrypt(int pair, KTByteArray& data, KTByteArray& cipher, int *retval);
	int ECCSign(int pair, KTByteArray& data, KTByteArray& cipher, int *retval);
	int PrivateKeyDecrypt(int pair, KTByteArray& cipher, KTByteArray& data, int *retval);
	int ECCDecrypt(int pair, KTByteArray& cipher, KTByteArray& data, int *retval);

	int EnumFiles(KTStringArray& fileList, int *retval);
	int CreateFile(KTString fileName, int fileSize, int readRights, int writeRights, int *retval);
	int DeleteFile(KTString fileName, int *retval);
	int GetFileInfo(KTString fileName, FILEATTRIBUTE *attribute, int *retval);
	int ReadFile(KTString fileName, int offset, int size, KTByteArray& data, int *retval);
	int WriteFile(KTString fileName, int offset, KTByteArray& data, int size, int *retval);

	int ImportSessionKey(int algID, KTByteArray& wrapedData, HANDLE *phKey, int *retval);
	int SetSymmKey(int algID, KTByteArray& symmKey, HANDLE *phKey, int *retval);
	int EncryptInit(HANDLE hKey, KTByteArray& iv, int paddingType, int feedBitLen, int *retval);
	int EncryptUpdate(HANDLE hKey, KTByteArray& data, KTByteArray& cipher, int *retval);
	int EncryptFinal(HANDLE hKey, KTByteArray& cipher, int *retval);
	int DecryptInit(HANDLE hKey, KTByteArray& iv, int paddingType, int feedBitLen, int *retval);
	int DecryptUpdate(HANDLE hKey, KTByteArray& cipher, KTByteArray& data, int *retval);
	int DecryptFinal(HANDLE hKey, KTByteArray& data, int *retval);
	int CloseHandle(HANDLE hKey, int *retval);

	int PacketEncrypt(int algID, KTByteArray& symmKey, KTByteArray& iv, KTByteArray& data, KTByteArray& cipher, int *retval);
	int PacketDecrypt(int algID, KTByteArray& symmKey, KTByteArray& iv, KTByteArray& cipher, KTByteArray& data, int *retval);

	int DigestInit(int algID, KTByteArray& pubkeyX, KTByteArray& pubkeyY, KTByteArray& userID, HANDLE *phHash, int *retval);
	int DigestUpdate(HANDLE hHash, KTByteArray& data, int *retval);
	int DigestFinal(HANDLE hHash, KTByteArray& cipher, int *retval);
	int GroupDigest(int algID, KTByteArray& pubkeyX, KTByteArray& pubkeyY, KTByteArray& userID, KTByteArray& data, KTByteArray& cipher, int *retval);

	int CreateCertReq(int pair, bool createNewContainer, bool updateKeyPair, KTString dn, KTByteArray& certReq, int *retval);
	int ECCCreateCertReq(int pair, bool createNewContainer, bool updateKeyPair, KTString dn, KTByteArray& certReq, int *retval);

	int GetPubKey(int pair, KTByteArray& pubkey, int *retval);
	int ECCGetPubKey(int pair, KTByteArray& pubkey, int *retval);
	int GetTmpPubKey(KTByteArray& pubkey, int *retval);
	int ECCGetTmpPubKey(KTByteArray& pubkey, int *retval);

	int ImportKeyPair(int pair, KTByteArray& cert, KTByteArray& prvkey, int *retval);
	int ECCImportKeyPair(int pair, KTByteArray& cert, KTByteArray& prvkey, int *retval);
};

#endif //__DEVICE0_H_
