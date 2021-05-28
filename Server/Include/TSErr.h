#ifndef _TSAERR_H_
#define _TSAERR_H_

#define TS_MemErr	   600	 //内存错误
#define TS_HashErr	   601	 //哈希运算错误     
#define TS_SetVerErr       602   //设置请求版本错误
#define TS_ANSIErr         603   //ANSI对象错误
#define TS_MSGAlgoErr      604	 //设置MSG算法错误     
#define TS_MSGErr          605   //设置MSG内容错误
#define TS_MSGImpErr       606   //设置MSG标记错误
#define TS_GenRandErr      607   //生成随机数错误
#define TS_SetRandErr      608   //设置随机数错误
#define TS_NewReqErr       609   //生成时间戳请求数据错误
#define TS_ReqErr          610   //时间戳请求数据非法
#define TS_RootCACertErr   611   //根CA证书错误
#define TS_CACertErr       612	 //中级CA证书错误
#define TS_CertErr         613   //签名证书错误
#define TS_KeyErr          614   //私钥错误
#define TS_PolicyErr       615   //设置签名策略错误
#define TS_RespHashErr     616   //设置RESP哈希算法错误
#define TS_AccurErr        617   //设置时间准确度错误
#define TS_PreciErr        618   //设置时间精确度错误
#define TS_NewRespErr      619   //生成时间戳响应数据错误
#define TS_RespErr         620   //时间戳响应数据非法
#define TS_VerifyErr       621   //验证时间戳失败
#define TS_TstErr          622   //获取TST信息失败
#define TS_B64EnErr        623   //B64编码失败
#define TS_B64DeErr        624   //B64解码失败
#define TS_BEFOREVALID     625   //服务器证书未生效
#define TS_AFTERVALID			 626   //服务器证书已过期

#endif
