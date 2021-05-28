#ifndef __TSA_COMMONUTIL_H__
#define __TSA_COMMONUTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "XmlDocument.h"
#include "MailCoder.h"
#include "locker.h"
#include "time.h"

using namespace std;

class CTSACommonUtil
{
public:
    CTSACommonUtil();
    ~CTSACommonUtil();

public:
    string getSettingValue_XML(string config_name);
    static string to_string(int n);
    static string to_hexstring(const char *pcstr);

private:
    //XML Setting
    string m_sSettingFilePath;
    CXmlDocument xmlSetting;
	DOMElement *xmlElem;
	DOMElement *xmlRoot;

    //locker
    locker m_queuelocker;

};






#endif 