#include "TSACommonUtil.h"


CTSACommonUtil::CTSACommonUtil()
{
    //XML Setting
    m_sSettingFilePath = "../Setting/TSASetting.xml";
    xmlElem = NULL;
    xmlRoot = NULL;
    xmlSetting.Load( m_sSettingFilePath.c_str() );
    xmlRoot = xmlSetting.GetRootElement();
}

CTSACommonUtil::~CTSACommonUtil()
{
}

string CTSACommonUtil::getSettingValue_XML(string config_name)
{
    xmlElem = xmlSetting.GetFirstChild( xmlRoot, config_name.c_str() );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get %s Port Failed.\n", config_name.c_str() );
		return "";
	}
    string value = xmlSetting.GetElementAttribute( xmlElem, "Description" );
    printf("[Success] getSettingValue: %s=%s \n", config_name.c_str(), value.c_str());
	return value;
}


string CTSACommonUtil::to_string(int n)
{
    char str[64] = {0};
    sprintf(str, "%d", n);
    return str;
}

string CTSACommonUtil::to_hexstring(const char *pcstr)
{
    const char HEX[] = "0123456789abcdef";
	int len = strlen(pcstr);
	string lStr;
	for(int i=0; i < len; i++)
	{
		char lTemp = pcstr[i];
		lStr.push_back(HEX[lTemp/16]);
		lStr.push_back(HEX[lTemp%16]);
	}
	return lStr;
}