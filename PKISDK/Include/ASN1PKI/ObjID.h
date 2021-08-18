#ifndef __OBJ_ID_H__
#define __OBJ_ID_H__

/**
  * @Name: ObjID.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/09 15:o4
  * @Description: all pkcs Object ID macro
*/

////rsadsi
#define ObjID_rsadsi	                        "1 2 840 113549"

////pkcs
#define ObjID_pkcs                              ObjID_rsadsi" 1"

////pkcs#1 objID
#define ObjID_pkcs_1                            ObjID_pkcs" 1"
#define ObjID_rsaEncryption                     ObjID_pkcs_1" 1"
#define ObjID_rsaOAEPEncryptionSET              ObjID_pkcs_1" 6"
#define ObjID_id_RSAES_OAEP                     ObjID_pkcs_1" 7"
#define ObjID_id_mgf1                           ObjID_pkcs_1" 8"
#define ObjID_id_pSpecified                     ObjID_pkcs_1" 9"
#define ObjID_id_RSASSA_PSS                     ObjID_pkcs_1" 10"

#define ObjID_md2WithRSAEncryption              ObjID_pkcs_1" 2"
#define ObjID_md4WithRSAEncryption              ObjID_pkcs_1" 3"
#define ObjID_md5WithRSAEncryption              ObjID_pkcs_1" 4"
#define ObjID_sha1WithRSAEncryption             ObjID_pkcs_1" 5"
#define ObjID_sha256WithRSAEncryption           ObjID_pkcs_1" 11"
#define ObjID_sha384WithRSAEncryption           ObjID_pkcs_1" 12"
#define ObjID_sha512WithRSAEncryption           ObjID_pkcs_1" 13"

#define ObjID_id_dsa_with_sha1                  "1 2 840 10040 4 3"
#define ObjID_id_dsa                            "1 2 840 10040 4 1"
#define ObjID_id_sha1                           "1 3 14 3 2 26"
#define ObjID_id_MS_Signature_Algo				"1 3 14 3 2 29"
#define ObjID_id_md2                            "1 2 840 113549 2 2"
#define ObjID_id_md5                            "1 2 840 113549 2 5"

#define ObjID_id_desCDMF                        "1 2 840 113549 3 10"      
#define ObjID_secsig                            "1 3 14 3"                 
#define ObjID_id_desCBC                         "1 3 14 3 2 7"            

// ============================================================
// SM2 and SM3
// 1 SM2公钥
#define ObjID_id_ecPublicKey					"1 2 840 10045 2 1"

#define ObjID_id_gjmmglj						"1 2 156 197"		// 国家密码管理局
#define ObjID_id_gjmmbzjswyh					"1 2 156 10197"		// 国家密码标准技术委员会

// 2 公钥密码算法
// SM2椭圆曲线公钥密码算法
#define ObjID_id_gjmmglj_sm2					ObjID_id_gjmmglj" 1 301"
#define ObjID_id_gjmmbzjswyh_sm2				ObjID_id_gjmmbzjswyh" 1 301"

// SM2-1数字签名算法
#define ObjID_id_gjmmglj_sm2_sign				ObjID_id_gjmmglj" 1 301 1"
#define ObjID_id_gjmmbzjswyh_sm2_sign			ObjID_id_gjmmbzjswyh" 1 301 1"

// SM2-2密钥交换协议
#define ObjID_id_gjmmglj_sm2_keyexchange		ObjID_id_gjmmglj" 1 301 2"
#define ObjID_id_gjmmbzjswyh_sm2_keyexchange	ObjID_id_gjmmbzjswyh" 1 301 2"

// SM2-3公钥加密算法
#define ObjID_id_gjmmglj_sm2_encrypt			ObjID_id_gjmmglj" 1 301 3"
#define ObjID_id_gjmmbzjswyh_sm2_encrypt		ObjID_id_gjmmbzjswyh" 1 301 3"

// 3 杂凑算法
// SM3密码杂凑算法
#define ObjID_id_gjmmglj_sm3					ObjID_id_gjmmglj" 1 401"
#define ObjID_id_gjmmbzjswyh_sm3				ObjID_id_gjmmbzjswyh" 1 401"

#define ObjID_id_gjmmbzjswyh_sm3_no_key			ObjID_id_gjmmbzjswyh_sm3" 1"
#define ObjID_id_gjmmbzjswyh_sm3_with_key		ObjID_id_gjmmbzjswyh_sm3" 2"

// 4 组合运算算法
// 基于SM3算法和SM2算法的签名
#define ObjID_id_gjmmglj_sm3_with_sm2			ObjID_id_gjmmglj" 1 501"
#define ObjID_id_gjmmbzjswyh_sm3_with_sm2		ObjID_id_gjmmbzjswyh" 1 501"

// 基于SM3算法和RSA算法的签名
#define ObjID_id_gjmmglj_sm3_with_rsa			ObjID_id_gjmmglj" 1 504"
#define ObjID_id_gjmmbzjswyh_sm3_with_rsa		ObjID_id_gjmmbzjswyh" 1 504"
// ============================================================

//    joint-iso-itu-t(2) country(16) us(840) organization(1) gov(101) 
//    csor(3) nistalgorithm(4) modules(0) sha2(1)
#define ObjID_NIST_SHA2							"2 16 840 1 101 3 4 0 1"
//#define ObjID_id_sha256						ObjID_NIST_SHA2" 1"
#define ObjID_id_sha256                         "2 16 840 1 101 3 4 2 1"
#define ObjID_id_sha384							ObjID_NIST_SHA2" 2"
#define ObjID_id_sha512							ObjID_NIST_SHA2" 3"

////pkcs#7 objID
#define ObjID_pkcs_7                            ObjID_pkcs" 7"
#define ObjID_data                              ObjID_pkcs_7" 1"
#define ObjID_signedData                        ObjID_pkcs_7" 2"
#define ObjID_envelopedData                     ObjID_pkcs_7" 3"
#define ObjID_signedAndEnvelopedData            ObjID_pkcs_7" 4"
#define ObjID_digestedData                      ObjID_pkcs_7" 5"
#define ObjID_encryptedData                     ObjID_pkcs_7" 6"

////pkcs#12 objID
#define ObjID_pkcs_12	                        ObjID_pkcs" 12"
#define ObjID_pkcs_12PbeIds                  	ObjID_pkcs_12" 1" 
#define ObjID_pbeWithSHAAnd128BitRC4            ObjID_pkcs_12PbeIds" 1"
#define ObjID_pbeWithSHAAnd40BitRC4             ObjID_pkcs_12PbeIds" 2"
#define ObjID_pbeWithSHAAnd3_KeyTripleDES_CBC	ObjID_pkcs_12PbeIds" 3"
#define ObjID_pbeWithSHAAnd2_KeyTripleDES_CBC	ObjID_pkcs_12PbeIds" 4"
#define ObjID_pbeWithSHAAnd128BitRC2_CBC        ObjID_pkcs_12PbeIds" 5"
#define ObjID_pbewithSHAAnd40BitRC2_CBC         ObjID_pkcs_12PbeIds" 6"

#define ObjID_bagtypes			                ObjID_pkcs_12" 10 1"
#define ObjID_keyBag 	                        ObjID_bagtypes" 1"
#define ObjID_pkcs8ShroudedKeyBag               ObjID_bagtypes" 2"
#define ObjID_certBag                           ObjID_bagtypes" 3"
#define ObjID_crlBag                            ObjID_bagtypes" 4"
#define ObjID_secretBag                         ObjID_bagtypes" 5"
#define ObjID_safeContentsBag                   ObjID_bagtypes" 6"

#define ObjID_x509Certificate                   ObjID_certTypes" 1"
#define ObjID_sdsiCertificate                   ObjID_certTypes" 2"
#define ObjID_x509CRL                           ObjID_crlTypes" 1"

////NetScape certification extension ObjID
#define ObjID_NetscapeCertType                  "2 16 840 1 113730 1 1"

////X509.h ObjID
#define ObjID_id_pkix                           "1 3 6 1 5 5 7"
#define ObjID_id_pe                             ObjID_id_pkix" 1"
        //   arc for private certificate extensions
#define ObjID_id_qt                             ObjID_id_pkix" 2"
        //   arc for policy qualifier types
#define ObjID_id_kp                             ObjID_id_pkix" 3"
        //   arc for extended key purpose OIDS
#define ObjID_id_ad                             ObjID_id_pkix" 48"
        //   arc for access descriptors
#define ObjID_id_qt_cps                         ObjID_id_qt" 1"
        //   OID for CPS qualifier
#define ObjID_id_qt_unotice                     ObjID_id_qt" 2"
        //   OID for user notice qualifier
#define ObjID_id_ad_ocsp                        ObjID_id_ad" 1"
#define ObjID_id_ad_caIssuers                   ObjID_id_ad" 2"
        
#define ObjID_id_pe_authorityInfoAccess			ObjID_id_pe" 1"

        //  Arc for standard naming attributes
#define ObjID_id_at                             "2 5 4"
#define ObjID_id_at_name                        ObjID_id_at" 41"
#define ObjID_id_at_surname                     ObjID_id_at" 4"
#define ObjID_id_at_givenName                   ObjID_id_at" 42"
#define ObjID_id_at_2541						ObjID_id_at" 1"
#define ObjID_id_at_initials                    ObjID_id_at" 43"
#define ObjID_id_at_generationQualifier         ObjID_id_at" 44"
#define ObjID_id_at_commonName                  ObjID_id_at" 3"
#define ObjID_id_at_localityName                ObjID_id_at" 7"
#define ObjID_id_at_stateOrProvinceName         ObjID_id_at" 8"
#define ObjID_id_at_organizationName            ObjID_id_at" 10"
#define ObjID_id_at_organizationalUnitName      ObjID_id_at" 11"
#define ObjID_id_at_title                       ObjID_id_at" 12"
#define ObjID_id_at_dnQualifier                 ObjID_id_at" 46"
#define ObjID_id_at_countryName                 ObjID_id_at" 6"
#define ObjID_id_at_postCode                    ObjID_id_at" 17"
#define ObjID_id_at_telephoneNumber             ObjID_id_at" 20"    
    
#define ObjID_id_at_dc							"0 9 2342 19200300 100 1 25"

#define ObjID_pkcs_9                            ObjID_pkcs" 9"
#define ObjID_emailAddress                      ObjID_pkcs_9" 1"

#define ObjID_id_dsa_with_sha1                  "1 2 840 10040 4 3"
#define ObjID_dhpublicnumber                    "1 2 840 10046 2 1"
#define ObjID_id_dsa                            "1 2 840 10040 4 1"
#define ObjID_PKIX1Implicit88                   "1 3 6 1 5 5 7 0 2"

#define ObjID_id_ce                             "2 5 29"
#define ObjID_id_ce_authorityKeyIdentifier      ObjID_id_ce" 35" 
#define ObjID_id_ce_subjectKeyIdentifier        ObjID_id_ce" 14"
#define ObjID_id_ce_keyUsage                    ObjID_id_ce" 15"
#define ObjID_id_ce_privateKeyUsagePeriod       ObjID_id_ce" 16"
#define ObjID_id_ce_certificatePolicies         ObjID_id_ce" 32"
#define ObjID_id_ce_policyMappings              ObjID_id_ce" 33"
#define ObjID_id_ce_subjectAltName              ObjID_id_ce" 17"
#define ObjID_id_ce_issuerAltName               ObjID_id_ce" 18"
#define ObjID_id_ce_subjectDirectoryAttributes  ObjID_id_ce" 9"
#define ObjID_id_ce_basicConstraints            ObjID_id_ce" 19"
#define ObjID_id_ce_nameConstraints             ObjID_id_ce" 30"
#define ObjID_id_ce_policyConstraints           ObjID_id_ce" 36"
#define ObjID_id_ce_cRLDistributionPoints       ObjID_id_ce" 31"
#define ObjID_id_ce_extKeyUsage                 ObjID_id_ce" 37"

#define ObjID_id_kp_serverAuth                  ObjID_id_kp" 1"
#define ObjID_id_kp_clientAuth                  ObjID_id_kp" 2"
#define ObjID_id_kp_codeSigning                 ObjID_id_kp" 3"
#define ObjID_id_kp_emailProtection             ObjID_id_kp" 4"
#define ObjID_id_kp_ipsecEndSystem              ObjID_id_kp" 5"
#define ObjID_id_kp_ipsecTunnel                 ObjID_id_kp" 6"
#define ObjID_id_kp_ipsecUser                   ObjID_id_kp" 7"
#define ObjID_id_kp_timeStamping                ObjID_id_kp" 8"

#define ObjID_id_ce_cRLNumber                   ObjID_id_ce" 20"
#define ObjID_id_ce_issuingDistributionPoint    ObjID_id_ce" 28"
#define ObjID_id_ce_deltaCRLIndicator           ObjID_id_ce" 27"
#define ObjID_id_ce_cRLReasons                  ObjID_id_ce" 21"
#define ObjID_id_ce_certificateIssuer           ObjID_id_ce" 29"
#define ObjID_id_ce_holdInstructionCode         ObjID_id_ce" 23"

#define ObjID_holdInstruction                   "2 2 840 10040 2"
//   ANSI X9 holdinstructions referenced by this standard
#define ObjID_id_holdinstruction_none           ObjID_holdInstruction" 1"
#define ObjID_id_holdinstruction_callissuer     ObjID_holdInstruction" 2"
#define ObjID_id_holdinstruction_reject         ObjID_holdInstruction" 3"

#define ObjID_id_ce_invalidityDate              ObjID_id_ce" 24"

////OCSP ObjID
#define ObjID_id_kp_OCSPSigning                 ObjID_id_kp" 9"
#define ObjID_id_pkix_ocsp                      ObjID_id_ad_ocsp
#define ObjID_id_pkix_ocsp_basic                ObjID_id_pkix_ocsp" 1"
#define ObjID_id_pkix_ocsp_nonce                ObjID_id_pkix_ocsp" 2"
#define ObjID_id_pkix_ocsp_crl                  ObjID_id_pkix_ocsp" 3"
#define ObjID_id_pkix_ocsp_response             ObjID_id_pkix_ocsp" 4"
#define ObjID_id_pkix_ocsp_nocheck              ObjID_id_pkix_ocsp" 5"
#define ObjID_id_pkix_ocsp_archive_cutoff       ObjID_id_pkix_ocsp" 6"
#define ObjID_id_pkix_ocsp_service_locator      ObjID_id_pkix_ocsp" 7"
#define ObjID_id_pkix_ocsp_custom			    ObjID_id_pkix_ocsp" 9"

////cert request message format ObjID
#define ObjID_id_pkip                           ObjID_id_pkix" 5"
#define ObjID_id_regCtrl                        ObjID_id_pkip" 1"
#define ObjID_id_regCtrl_authenticator          ObjID_id_regCtrl" 2"
#define ObjID_id_regCtrl_pkiPublicationInfo     ObjID_id_regCtrl" 3"
#define ObjID_id_regCtrl_pkiArchiveOptions      ObjID_id_regCtrl" 4"
#define ObjID_id_regCtrl_oldCertID              ObjID_id_regCtrl" 5"
#define ObjID_id_regCtrl_protocolEncrKey        ObjID_id_regCtrl" 6"

#define ObjID_id_regInfo                        ObjID_id_pkip" 2"
#define ObjID_id_regInfo_utf8Pairs              ObjID_id_regInfo" 1"
#define ObjID_id_regInfo_certReq                ObjID_id_regInfo" 2"

////cert management protocols
#define ObjID_PasswordBasedMac                  "1 2 840 113533 7 66 13"
#define ObjID_DHBasedMac                        "1 2 840 113533 7 66 30"
#define ObjID_id_it								ObjID_id_pkix" 4"
#define ObjID_id_it_1							ObjID_id_it" 1"
#define ObjID_id_it_2							ObjID_id_it" 2"
#define ObjID_id_it_3							ObjID_id_it" 3"
#define ObjID_id_it_4							ObjID_id_it" 4"
#define ObjID_id_it_5							ObjID_id_it" 5"
#define ObjID_id_it_6							ObjID_id_it" 6"

////pkcs9 object identifier

//#define ObjID_pkcs_9                            "1 2 840 113549 1 9" 
  //   Main arcs
#define ObjID_pkcs_9_mo	                         ObjID_pkcs_9" 0"
#define ObjID_pkcs_9_oc    	                     ObjID_pkcs_9" 24"
#define ObjID_pkcs_9_at                          ObjID_pkcs_9" 25"
#define ObjID_pkcs_9_sx	                         ObjID_pkcs_9" 26"
#define ObjID_pkcs_9_mr                          ObjID_pkcs_9" 27"

  //   Object classes
#define ObjID_pkcs_9_oc_pkcsEntity   			 ObjID_pkcs_9_oc" 1"
#define ObjID_pkcs_9_oc_naturalPerson            ObjID_pkcs_9_oc" 2"

  //   Attributes
#define ObjID_pkcs_9_at_emailAddress             ObjID_pkcs_9" 1"
#define ObjID_pkcs_9_at_unstructuredName         ObjID_pkcs_9" 2"
#define ObjID_pkcs_9_at_contentType              ObjID_pkcs_9" 3"
#define ObjID_pkcs_9_at_messageDigest            ObjID_pkcs_9" 4"
#define ObjID_pkcs_9_at_signingTime              ObjID_pkcs_9" 5"
#define ObjID_pkcs_9_at_counterSignature         ObjID_pkcs_9" 6"
#define ObjID_pkcs_9_at_challengePassword        ObjID_pkcs_9" 7"
#define ObjID_pkcs_9_at_unstructuredAddress      ObjID_pkcs_9" 8"
#define ObjID_pkcs_9_at_extendedCertificateAttributes   ObjID_pkcs_9" 9"

//   Obsolete (?) attribute identifiers, purportedly from "tentative
//   PKCS #9 draft"
#define ObjID_pkcs_9_at_issuerAndSerialNumber    ObjID_pkcs_9" 10"
#define ObjID_pkcs_9_at_passwordCheck            ObjID_pkcs_9" 11"
#define ObjID_pkcs_9_at_publicKey                ObjID_pkcs_9" 12"

#define ObjID_pkcs_9_at_signingDescription       ObjID_pkcs_9" 13"
#define ObjID_pkcs_9_at_extensionRequest         ObjID_pkcs_9" 14"
#define ObjID_pkcs_9_at_smimeCapabilities        ObjID_pkcs_9" 15"

//   Unused (?)
//   pkcs 9 at ?                                 ObjID_pkcs_9" 17"
//   pkcs_9_at_?                                 ObjID_pkcs_9" 18"
//   pkcs_9_at_?                                 ObjID_pkcs_9" 19"

#define ObjID_pkcs_9_at_friendlyName             ObjID_pkcs_9" 20"
#define ObjID_pkcs_9_at_localKeyId               ObjID_pkcs_9" 21"
#define ObjID_pkcs_9_at_userPKCS12               "2 16 840 1 113730 3 1 216"
#define ObjID_pkcs_9_at_pkcs15Token              ObjID_pkcs_9_at" 1"
#define ObjID_pkcs_9_at_encryptedPrivateKeyInfo  ObjID_pkcs_9_at" 2"
#define ObjID_pkcs_9_at_randomNonce              ObjID_pkcs_9_at" 3"
#define ObjID_pkcs_9_at_sequenceNumber           ObjID_pkcs_9_at" 4"
#define ObjID_pkcs_9_at_pkcs7PDU                 ObjID_pkcs_9_at" 5"

  //   IETF PKIX Attribute branch
#define ObjID_ietf_at         			         "1 3 6 1 5 5 7 9"

#define ObjID_pkcs_9_at_dateOfBirth              ObjID_ietf_at" 1"
#define ObjID_pkcs_9_at_placeOfBirth             ObjID_ietf_at" 2"
#define ObjID_pkcs_9_at_gender                   ObjID_ietf_at" 3"
#define ObjID_pkcs_9_at_countryOfCitizenship     ObjID_ietf_at" 4"
#define ObjID_pkcs_9_at_countryOfResidence       ObjID_ietf_at" 5"

  //   Syntaxes (for use with LDAP accessible directories)
#define ObjID_pkcs_9_sx_pkcs9String              ObjID_pkcs_9_sx" 1"
#define ObjID_pkcs_9_sx_signingTime              ObjID_pkcs_9_sx" 2"

  //   Matching rules
#define ObjID_pkcs_9_mr_caseIgnoreMatch          ObjID_pkcs_9_mr" 1"
#define ObjID_pkcs_9_mr_signingTimeMatch         ObjID_pkcs_9_mr" 2"

  //   Arcs with attributes defined elsewhere
#define ObjID_smime             			     ObjID_pkcs_9" 16"
  //   Main arc for S/MIME (RFC 2633)
#define ObjID_certTypes         			     ObjID_pkcs_9" 22"
  //   Main arc for certificate types defined in PKCS #12
#define ObjID_crlTypes          			     ObjID_pkcs_9" 23"
  //   Main arc for crl types defined in PKCS #12

  //   Other object identifiers
#define ObjID_id_at_pseudonym				     ObjID_id_at" 65"

////Microsoft CSP
#define ObjID_Microsoft							 "1 3 6 1 4 1"
#define ObjID_Microsoft_CSP_Name                 ObjID_Microsoft" 311 17 1"
#define ObjID_Microsoft_enrolmentCSP			 ObjID_Microsoft" 311 13 2 2"
#define ObjID_Microsoft_osVersion				 ObjID_Microsoft" 311 13 2 3"
#define ObjID_Microsoft_certReqExtensions		 ObjID_Microsoft" 311 2 1 14"

////pkcs#5
//rsadsi OBJECT IDENTIFIER ::= 
//  {iso(1) member-body(2) us(840) 113549}
#define ObjID_pkcs_5							 ObjID_pkcs" 5"
#define ObjID_id_PBKDF2							 ObjID_pkcs_5" 12"

#define ObjID_pbeWithMD2AndDES_CBC 				 ObjID_pkcs_5" 1"
#define ObjID_pbeWithMD2AndRC2_CBC 				 ObjID_pkcs_5" 4"
#define ObjID_pbeWithMD5AndDES_CBC 				 ObjID_pkcs_5" 3"
#define ObjID_pbeWithMD5AndRC2_CBC 				 ObjID_pkcs_5" 6"
#define ObjID_pbeWithSHA1AndDES_CBC 			 ObjID_pkcs_5" 10"
#define ObjID_pbeWithSHA1AndRC2_CBC 			 ObjID_pkcs_5" 11"

#define ObjID_id_PBES2							 ObjID_pkcs_5" 13"
#define ObjID_id_PBMAC1							 ObjID_pkcs_5" 14"

#define ObjID_digestAlgorithm					 ObjID_rsadsi" 2"
#define ObjID_encryptionAlgorithm				 ObjID_rsadsi" 3"
#define ObjID_id_hmacWithSHA1					 ObjID_digestAlgorithm" 7"

//desCBC OBJECT IDENTIFIER ::=
//  {iso(1) identified-organization(3) oiw(14) secsig(3)
//    algorithms(2) 7} -- from OIW
#define ObjID_des_EDE3_CBC						 ObjID_encryptionAlgorithm" 7"
#define ObjID_rc2CBC							 ObjID_encryptionAlgorithm" 2"
#define ObjID_rc5_CBC_PAD						 ObjID_encryptionAlgorithm" 9"

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------                                                                        
///   1.0.0.0  2003/04/14 18:29   Hongwei Yu   created 

#endif
