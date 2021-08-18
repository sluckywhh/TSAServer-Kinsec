#ifndef __PARAMPARSE_H__
#define __PARAMPARSE_H__

/**
  * @Name: ParamParser.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 23:52
  * @Description: parse parameters
  * @Example:"RSA/512"."RSA/ENCRYPT/PKCS1" etc.
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

class CParamParser  
{
public:
	CParamParser(const char *str);
	virtual ~CParamParser();
	
	/**
	* get count of part.
	*/
	int getCount();
	/**
	* get value of part.
	*/
	const char * operator[](int n)const;
	/**
	* find a value.
	* @param value to be found.
	* @return the position if found.
	*		  -1 if not found.
	*/
	int find(const char *str);

protected:
	void readData(const char *str);
	KTStringArray m_rValue;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 23:52  Hongwei Yu   created 

#endif 
