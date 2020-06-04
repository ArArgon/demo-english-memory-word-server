#ifndef PARSE_HTTP_H_
#define PARSE_HTTP_H_

#include <iostream>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>

using std::string;
using std::map;
using std::vector;

class ParseHttpRequest {
private:
    map<string, string> requestHeaders;
    string requestMethod;
    string requestPath;
    map<string, string> requestQuery;
    map<string, string> requestParam;
    
public:
    //分割请求头，字符串处理操作，将请求信息存入对象中
    ParseHttpRequest(const string& httpRequest);
    
    static string urlDecode(string url);
    static string urlEncode(string url);
    static string base64Encode(string str);
    static string base64Decode(string str);

    string getRequestHeader(string headerName) { return requestHeaders[headerName];}
    string getRequestMethod() { return requestMethod; }
    string getRequestPath() { return requestPath; }
    string getRequestParam(string paramName) { return requestParam[paramName]; }
    string getRequestQuery(string queryName) { return requestQuery[queryName]; }
    map<string, string> getAllRequestQueries() { return requestParam; }
};


class MakeHttpResponse {
private:
    map<string, string> responseHeaders;
    int code;
    string responseBody;
    
public:
    MakeHttpResponse() : code(0) { }

    MakeHttpResponse* setResponseHeader(string headerName, string headerValue) {
        responseHeaders[headerName] = headerValue;
        return this;
    }

    MakeHttpResponse* setCode(int _code) : code(_code) {
        return this;
    }

    MakeHttpResponse* setResponseBody(string _responseBody) : responseBody(_responseBody) {
        return this;
    }

    string generateHttpResponse();
};
#endif
