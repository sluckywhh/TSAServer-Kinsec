#include "http_function.h"

void PrintMap(map<string, string> mdata)
{
    printf("------------PrintMap start------------\n");
    map<string, string>::iterator it;
    for (it = mdata.begin(); it != mdata.end(); ++it)
    {
        printf("[%s] = %s \n", it->first.c_str(), it->second.c_str());
    }
    printf("------------PrintMap end------------\n");
}

http_function::http_function(char *fname, char *rdata)
{
    printf("new http_function: fname:%s\n", fname);
    printf("new http_function: rdata:%s\n", rdata);
    this->function_name = fname;
    this->request_data = rdata;
    this->response_data = "";
}

http_function::~http_function()
{
    
}
/*********************************************************************************************/
/*********************************************************************************************/

//函数名转换
int http_function::FunctionName2FunctionDefine()
{   
    if(function_name == "TSAInitEnvironment") return TSA_INIT_ENVIRONMENT;
    else if(function_name == "TSAClearEnvironment") return TSA_CLEAR_ENVIRONMENT;
    else if(function_name == "TSACreateTSRequest") return TSA_CREATE_TS_REQUEST;
    else if(function_name == "TSACreateTSResponse") return TSA_CREATE_TS_RESPONSE;
    else if(function_name == "TSAVerifyTSValidity") return TSA_VERIFY_TS_VALIDITY;
    else if(function_name == "TSAGetTSInfo") return TSA_GET_TS_INFO;
    else if(function_name == "TSAGetTSDetail") return TSA_GET_TS_DETAIL;
}


//功能处理
bool http_function::FunctionProcess(char *response_body)
{
    printf("FunctionProcess start\n");
    ParseRequestData();
    PrintMap(mrequestdata);
    int fdefine = FunctionName2FunctionDefine();
    printf("FunctionProcess...fdefine=%d\n", fdefine);

    int retCode = TSAFunctionAssign(fdefine, mrequestdata, mresponsedata);
    if (SAR_OK != retCode){
        printf("FunctionProcess error: %d\n", retCode);
        return false;
    }

    EncapsulationResponseData();
    printf("FunctionProcess response_data===%s\n", response_data.c_str());
    strcpy( response_body, response_data.c_str() );

    return true;
}

//解析请求数据
//data1=111&data2=222&data3=333
bool http_function::ParseRequestData()
{
    //分割"&"
    int find_pos = 0, start_pos = 0;
    find_pos = request_data.find("&", start_pos);
    while (find_pos != string::npos)
    {
        string skv = request_data.substr(start_pos, find_pos-start_pos);
        //分割"="
        int find_pos2 = skv.find("=");
        string key = skv.substr(0, find_pos2);
        string value = skv.substr(find_pos2+1);
        mrequestdata[key] = value;
        //printf("ParseRequestData...mrequestdata[%s]===%s\n", key.c_str(), value.c_str());

        start_pos = find_pos + 1;
        find_pos = request_data.find("&", start_pos);
    }
    string skv = request_data.substr(start_pos);
    //分割"="
    int find_pos2 = skv.find("=");
    string key = skv.substr(0, find_pos2);
    string value = skv.substr(find_pos2+1);
    mrequestdata[key] = value;
    //printf("ParseRequestData...mrequestdata[%s]===%s\n", key.c_str(), value.c_str());

    return true;
}

//封装响应数据
bool http_function::EncapsulationResponseData()
{
    map<string, string>::iterator it;
    for (it = mresponsedata.begin(); it != mresponsedata.end(); ++it)
    {
        if (it == mresponsedata.begin()) response_data += it->first + "=" + it->second;
        else response_data += "&" + it->first + "=" + it->second;
    }

    return true;
}


