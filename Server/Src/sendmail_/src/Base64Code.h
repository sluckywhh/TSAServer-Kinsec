#include <string.h>

#ifndef LPSTR
    #define LPSTR char*
#endif

#ifndef LPCSTR
    #define LPCSTR const char*
#endif

#define FALSE false
#define TRUE  true

#define DWORD unsigned long
#define BYTE unsigned char
#define PBYTE unsigned char*
#ifndef PAGESIZE
#define PAGESIZE					4096
#endif

#ifndef ROUNDTOPAGE
#define ROUNDTOPAGE(a)			(((a/4096)+1)*4096)
#endif

#ifndef __H_BASE64CODE_H
#define __H_BASE64CODE_H
class CBase64Coder  
{
public:
	CBase64Coder();
    ~CBase64Coder();

//方法
	void	 Encode(const PBYTE pBuffer, DWORD nBufLen);
	void	 Decode(const PBYTE pBuffer, DWORD nBufLen);
	void	 Encode(LPCSTR sMessage);
	void	 Decode(LPCSTR sMessage);
	LPSTR    DecodedMessage() const;
	LPSTR    EncodedMessage() const;
	long	 DecodedMessageSize() const;
	long	 EncodedMessageSize() const;

protected:
	class TempBucket
	{
	public:
		BYTE		nData[4];
		BYTE		nSize;
		void		Clear() { memset(nData, 0, 4); nSize = 0; };                                  
	};
	PBYTE	m_pDBuffer;
	PBYTE	m_pEBuffer;
	DWORD	m_nDBufLen;
	DWORD	m_nEBufLen;
	DWORD	m_nDDataLen;
	DWORD	m_nEDataLen;
	static char m_DecodeTable[256];
	static bool m_Init;

//M方法
	void	 AllocEncode(DWORD nSize);
	void	 AllocDecode(DWORD nSize);
	void	 SetEncodeBuffer(const PBYTE pBuffer, DWORD nBufLen);
	void	 SetDecodeBuffer(const PBYTE pBuffer, DWORD nBufLen);
	void	_EncodeToBuffer(const TempBucket &Decode, PBYTE pBuffer);
	DWORD	_DecodeToBuffer(const TempBucket &Decode, PBYTE pBuffer);
	void	_EncodeRaw(TempBucket &, const TempBucket &);
	void	_DecodeRaw(TempBucket &, const TempBucket &);
	bool    _IsBadMimeChar(BYTE);
	void	_Init();
};
#endif


