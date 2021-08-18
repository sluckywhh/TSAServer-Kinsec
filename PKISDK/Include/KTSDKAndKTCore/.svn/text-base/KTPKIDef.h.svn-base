#ifndef _KTPKI_DEF_H
#define _KTPKI_DEF_H

enum enumCertStatus
{
	StatusOK,    			// 证书没有问题
	StatusBeforeinEffect, 	// 证书尚未生效
	StatusExpired,   		// 证书已经过期
	StatusTimeBeforeIssuer, // 证书比发行者先生效
	StatusTimeAfterIssuer,	// 证书比发行者后作废
	StatusChainBad,   		// 没有有效证书链
	StatusUnAuthorized,  	// 签名验证失败
	StatusParentCertError, 	// 上级证书未通过验证
	StatusRevoked,   		// 证书已被废除
	StatusUnspecify,		// 未指定错误
	StatusOKWithTSS,		// 证书没有问题,并且支持时间戳
	StatusTSSCertNotTrust	// 时间戳证书不受信任
};

#endif