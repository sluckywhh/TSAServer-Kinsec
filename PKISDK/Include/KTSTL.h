#ifndef _KT_STL_H_
#define _KT_STL_H_

/**
  *@Name: KTStd.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/03 15:35
  *@Description: all KTXA std class Type 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTType.h"

#ifdef	__cplusplus

#ifdef KT_WIN32  
	#pragma warning(disable:4786)
#endif

	#include <vector>
	#include <string>
	#include <list>
	#include <iomanip>
	#include <istream>
	#include <ostream>
	#include <iostream>
	#include <fstream>
    #include <strstream>
    #include <sstream>
    #include <map>
    #include <algorithm>

    typedef std::string             KTString;
    typedef std::vector<KTINT>      KTIntArray;
    typedef std::vector<KTUINT>     KTUintArray;
    typedef std::vector<KTCHAR>     KTCharArray;
	typedef std::vector<KTBYTE> 	KTByteArray;
    typedef std::vector<KTBOOL>     KTBoolArray;
	typedef std::vector<KTINT16> 	KTInt16Array;
	typedef std::vector<KTINT32> 	KTInt32Array;
	typedef std::vector<KTUINT16>   KTUint16Array;
	typedef std::vector<KTUINT32>   KTUint32Array;
	typedef std::vector<KTHANDLE>   KTHandleArray;
	typedef std::vector<KTString>   KTStringArray;
	typedef std::vector<KT_ALGORITHM_INFO> KTAlgoInfoArray;
	
	typedef std::strstream      KTStrStream;
	typedef std::streambuf      KTStreamBuf;
	typedef std::fstream        KTFileStream;
	typedef std::ifstream       KTInFileStream;
	typedef std::ofstream       KTOutFileStream;
	typedef std::istringstream  KTInStrStream;
	typedef std::ostringstream  KTOutStrStream;

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/03 15:36  Hongwei Yu   created 

#endif
