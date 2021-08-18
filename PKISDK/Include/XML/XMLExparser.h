#ifndef _XMLEXPARSER_H_
#define _XMLEXPARSER_H_

/**
  * @Name: XMLExparser.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 14:52
  * @Description: interface for the CXMLConfig class
*/

#include <stdio.h>
#include "xmlparse.h"
#include <string.h>
#include <stdlib.h>

//#define MAX_REG_VALUE_LENGTH 255
#define MAX_REG_VALUE_LENGTH 512
#define MAX_REG_NAME_LENGTH	255

#define STR_XML_FORMAT_ERR "配置文件%s有错误: 第%d行%s。\n备注：请联系您的系统管理员。\n\n",

enum
{ 
	XML_ERROR_SUCCESS=0,		//OK
	XML_ERROR_PARSE,			// error occures in parsing file
	XML_ERROR_OPENFILE,			//open xml file error
	XML_ERROR_READFILE,			// read xml file error
	XML_ERROR_SAVEFILE,			//save xml file error
	XML_ERROR_KEY_NOTEXIST,		//The key that you want to open doesn't exist
	XML_ERROR_VALUE_NOTEXIST,	//The value you want to query doesn't exist
	XML_ERROR_SET_VALUE	,		//cannot set value
	XML_ERROR_DELKEY			//cannot delete key
};

typedef void *HXMLKEY;
typedef HXMLKEY *PHXMLKEY;


//##ModelId=3B71F345032E
class CXMLConfig  
{
struct tagXMLStack
{
	char Name[MAX_REG_NAME_LENGTH+1];
	int  NameLength;
	char Value[MAX_REG_VALUE_LENGTH+1];
	int ValueLength;
	int  childrens;
	int level;
	int isValue;
	struct tagXMLStack *parent;
	struct tagXMLStack *next;
};

typedef struct tagXMLStack XMLStack;
public:
	//##ModelId=3B71F4220245
	CXMLConfig();
	//##ModelId=3B71F42202A9
	CXMLConfig(const char *fileName);
	//##ModelId=3B71F42201B9
	virtual ~CXMLConfig();
	//##ModelId=3B71F419029C
	friend void characterDataHandler(void *userData,const char *strValue,int len);
	//##ModelId=3B71F41A0045
	friend void startElement(void *userData, const char *name, const char **atts);
	//##ModelId=3B71F41A018F
	friend void endElement(void *userData, const char *name);

	//##ModelId=3B71F42200D3
	int ParserXMLFile(const char *fileName = NULL, bool bCreateFileIfError=false);
	
	// functions used with xml stack  
	//##ModelId=3B71F4210320
	int XmlOpenKey(const char *lpSubkey,PHXMLKEY lpResult);

	HXMLKEY XmlOpendKey(){ return w_stack; };
	
	const char* FileName() { return w_chFileName; };

	//##ModelId=3B71F42100EF
	int XmlOpenSubKey(HXMLKEY hKey,const char *lpSubkey,PHXMLKEY lpResult);

	//##ModelId=3B71F42002C4 //Hongwei Yu: queried the value at opend key moment
	int XmlQueryValue(HXMLKEY hKey,const char *lpValueName,char *lpData);

	//##ModelId=3B71F41F0395
	int XmlEnumKey(HXMLKEY hKey,int index,char *lpName, PHXMLKEY lpResult);

//	int XmlEnumKey(HXMLKEY hKey,int index, PHXMLKEY lpResult);

	//##ModelId=3B71F41F00A6
	int XmlEnumValue(HXMLKEY hKey,int index,char *lpValueName,char *lpValue);

	//##ModelId=3B71F41E0271
	int XmlCreateKey(HXMLKEY hKey,const char *lpSubkey,PHXMLKEY lpResult);

	//##ModelId=3B71F41E0109
	int XmlDeleteKey(HXMLKEY hKey,const char *lpSubkey);

	int XmlDeleteSubKey(HXMLKEY hKey,HXMLKEY hSubkey);

	//##ModelId=3B71F41D0388
	int XmlDeleteValue(HXMLKEY hKey,const char *lpValueName);

	//##ModelId=3B71F41D01A8
	int XmlSetValue(HXMLKEY hKey,const char *lpValueName,const char *lpValue);

	int XmlSetValue(HXMLKEY hKey,const char *lpValue);

	//##ModelId=3B71F41D0162
	int XmlRefresh(bool bCreateDefault=false);

	//##ModelId=3B71F41D008F
	int SubKeys(HXMLKEY hKey)
	{	return ((XMLStack *)hKey)->childrens;	}


#ifdef _DEBUG
	//##ModelId=3B71F41C0283
	int GetValue(char *value,char *name,int num=0);
	//##ModelId=3B71F41C0233
	void WorkThroughStack();
#endif

//	int DeleteFromStack(XMLStack *tag);
protected:
	// Three functions used to maintain stack
	//##ModelId=3B71F41C0160
	int AddToStack(XMLStack *tag);
	//##ModelId=3B71F41C0098
	void WriteContent(char *content);
	//##ModelId=3B71F41C0048
	void EndElement();
	//##ModelId=3B71F41B0368
	void StartElement(const char *name);
	

private:
	//##ModelId=3B71F41A0384
	XMLStack *w_stack;
	//##ModelId=3B71F41A0368
	XMLStack *w_parent;
	//##ModelId=3B71F41A0352
	XMLStack *w_stackpoint;
	//##ModelId=3B71F41A02D0
	char *w_chFileName;//[_MAX_PATH];
	//##ModelId=3B71F41A0280
	int w_bModified;

private:
	//##ModelId=3B71F41B0317
	void FreeStack();
	//##ModelId=3B71F41B01CD
	int ParseName(const char *name,char *parsed);
	//##ModelId=3B71F41B00FB
	void DeleteSubkey(XMLStack * toDelete);
	//##ModelId=3B71F41A038E
	int Save(FILE *f,XMLStack * key);
};

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 14:53  Hongwei Yu   created
///   1.0.0.1  2003/12/08 10:34  Hongwei Yu   added function XMLOpendKey and FileName 

#endif // end of not defined _XMLEXPARSER_H_
