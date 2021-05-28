#ifndef _HTTP_FUNCTION_H_
#define _HTTP_FUNCTION_H_

#include <string>
#include <map>
#include "STFTSAFunction.h"

using namespace std;

class http_function
{
public:
    http_function(char *fname, char *rdata);
    ~http_function();

public:
    int FunctionName2FunctionDefine();  //函数名转换
    bool FunctionProcess(char *response_body); //功能处理
    //解析请求数据
    //data1=111&data2=222&data3=333
    bool ParseRequestData();
    //封装响应数据
    bool EncapsulationResponseData();

private:
    string function_name;
    string request_data;
    string response_data;
    map<string, string> mrequestdata;
    map<string, string> mresponsedata;
};


#endif