#ifndef PARSE_HTTP_H_
#define PARSE_HTTP_H_
#include <iostream>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>


class ParseHttpRequest {
private:
    std::map<std::string, std::string> requestHeaders;
    std::string requestMethod;
    std::string requestPath;
    std::map<std::string, std::string> requestQuery;
    std::map<std::string, std::string> requestParam;
public:
    //分割请求头，字符串处理操作，将请求信息存入对象中
    ParseHttpRequest(std::string httpRequest);
    static std::string urlDecode(std::string url);
    static std::string urlEncode(std::string url);
    static std::string base64Encode(std::string str);
    static std::string base64Decode(std::string str);

    std::string getRequestHeader(std::string headerName){ return this->requestHeaders[headerName];}
    std::string getRequestMethod(){ return this->requestMethod; }
    std::string getRequestPath(){ return this->requestPath; }
    std::string getRequestParam(std::string paramName){ return this->requestParam[paramName]; }
    std::string getRequestQuery(std::string queryName){ return this->requestQuery[queryName]; }
    std::map<std::string, std::string> getAllRequestQueries(){ return this->requestParam; }
};


class MakeHttpResponse {
private:
    std::map<std::string, std::string> responseHeaders;
    int code;
    std::string responseBody;
public:
    MakeHttpResponse(){}

    MakeHttpResponse* setResponseHeader(std::string headerName, std::string headerValue){
        this->responseHeaders[headerName] = headerValue;
        return this;
    }

    MakeHttpResponse* setCode(int code){
        this->code = code;
        return this;
    }

    MakeHttpResponse* setResponseBody(std::string responseBody){
        this->responseBody = responseBody;
        return this;
    }

    std::string generateHttpResponse();
};
#endif
