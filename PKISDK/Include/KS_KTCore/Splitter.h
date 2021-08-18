// Splitter.h : Declaration of the CSplitter

#ifndef __SPLITTER_H_
#define __SPLITTER_H_

// 打开待解密文件，获取上下文句柄
int KSSPLT_OpenContext(char *pszSourceFile, HANDLE *phHandle);
// 关闭上下文句柄
int KSSPLT_CloseContext(HANDLE hHandle);
// 获取签名者的通用名
int KSSPLT_QuerySignerName(HANDLE hHandle, char *pszName);
// 获取接收者个数
int KSSPLT_QueryReceivers(HANDLE hHandle, int *pnCount);
// 获取指定接收者的通用名
int KSSPLT_QueryReceiverName(HANDLE hHandle, int nIndex, char *pszName);
// 获取指定接收者的加密对称密钥
int KSSPLT_QueryESK(HANDLE hHandle, int nIndex, char *pszESK);
// 获取错误信息
void KSSPLT_GetErrString(HANDLE hHandle, char *pszErrString);

// 对称解密文件
int KSSPLT_DecryptWithSK(char *pszSourceFile, char *pszTargetFile, char *pszSK, char *pszErrString);

#endif //__SPLITTER_H_
