// 头文件顺序：相关头文件，c库，c++库，第三方库，项目内的头文件

#include <stdlib.h>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "parseHttp.h"

using std::vector;
using std::string;
using boost::split;

ParseHttpRequest::ParseHttpRequest(const string& httpRequest) {
    vector<string> http, tmp;
    string key, value;
    
    split(http, httpRequest, boost::is_any_of("\n"));
    
    vector<string> httpRequestLineParams;
    split(httpRequestLineParams, http[0], boost::is_any_of(" "));
    
    requestMethod = httpRequestLineParams[0];
    requestPath = httpRequestLineParams[1].substr(0, httpRequestLineParams[1].find("?"));
    
    string&& t = httpRequestLineParams[1].substr(httpRequestLineParams[1].find("?") + 1, httpRequestLineParams[1].size());
    split(tmp, t, boost::is_any_of("&"));
    
    for (auto& ele : tmp) {
        key = ele.substr(0, ele.find("="));
        value = urlDecode(ele.substr(ele.find("=") + 1, ele.size()));
        requestQuery[key] = value;
    }
    
//    for(int n = 0; n < tmp.size(); n++){
//        key = tmp[n].substr(0, tmp[n].find("="));
//        value = urlDecode(tmp[n].substr(tmp[n].find("=") + 1, tmp[n].size()));
//        this->requestQuery[key] = value;
//    }
    
    // 处理请求头信息
    
    for (auto it = http.begin(); it != http.end(); it++) {
        if (it->size() == 1) {
            for (auto itt = it; itt != http.end(); itt++) {
                split(tmp, *itt, boost::is_any_of("&"));
                for (auto& r : tmp)
                    requestParam[r.substr(0, r.find("="))] = r.substr(r.find("=") + 1, r.size());
            }
            break;
        }
        requestHeaders[it->substr(0, it->find(": "))] = it->substr(it->find(": ") + 2, it->size());
    }
//    for(int n = 1; n < http.size(); n++) {
//        if (http[n].size() == 1) {  // 进入请求体部分
//            for(int i = n + 1; i < http.size(); i++) {
//                split(tmp, http[i], boost::is_any_of("&"));
//                for(int r = 0; r < tmp.size(); r++)
//                    this->requestParam[tmp[r].substr(0, tmp[r].find("="))] = tmp[r].substr(tmp[r].find("=") + 1, tmp[r].size());
//            }
//            break;
//        }
//        this->requestHeaders[http[n].substr(0, http[n].find(": "))] = http[n].substr(http[n].find(": ") + 2, http[n].size());
//    }
}

 /*
 ?????????????????
 ?     ?????     ?
 ?    ??   ??    ?
 ?        ??     ?
 ?       ??      ?
 ?       ??      ?
 ?               ?
 ?       ??      ?
 ?????????????????
         🤔
 */

string ParseHttpRequest::urlDecode(string url){
    if (!url.size())
        return "";
    url = boost::algorithm::replace_all_copy(url, "\"", "\\\"");
    // ???
    string cmd = "python3 other/urldecode.py \"" + url + "\"";
    char output[256];
    FILE* pf = popen((char*)cmd.c_str(), "r");
    fgets(output, 256, pf);
    fclose(pf);
    return output;
}

string ParseHttpRequest::urlEncode(string url){
    // ???
    if (url.size() == 0) return "";
    url = boost::algorithm::replace_all_copy(url, "\"", "\\\"");
    string cmd = "python3 other/urlencode.py \"" + url + "\"";
    std::cout << cmd << std::endl;
    char output[256];
    FILE* pf = popen((char*)cmd.c_str(), "r");
    fgets(output, 256, pf);
    std::cout << output << std::endl;
    fclose(pf);
    return output;
}

string ParseHttpRequest::base64Decode(string str){
    /// ???
    string cmd = "echo \"" + str + "\" | base64 -d";
    FILE* p = popen((char*)cmd.c_str() , "r");
    char res[512];
    fgets(res, 512, p);
    return res;
}

string ParseHttpRequest::base64Encode(string str){
    /// ???
    str = boost::algorithm::replace_all_copy(str, "\"", "\\\"");
    string cmd = "echo \"" + str + "\" | base64 | tr '\\n' '['";
    FILE* p = popen((char*)cmd.c_str() , "r");
    char res[512];
    fgets(res, 512, p);
    string result = res;
    result = boost::algorithm::replace_all_copy(result, "[", "");
    return result;
}

string MakeHttpResponse::generateHttpResponse() {
    string response("HTTP/1.0 ");
    switch (this->code) {
        case 200:
            response += "200 OK\r\n";
            break;
        case 400:
            response += "400 Bad Request\r\n";
            break;
        case 404:
            response += "404 Not Found\r\n";
            break;
        case 405:
            response += "405 Methods Not Allowed\r\n";
            break;
        case 500:
            response += "500 Internal Server Error\r\n";
            break;
        default:
            response += "500 Internal Server Error\r\n";
            break;
    }
    for(auto it = responseHeaders.begin(); it != responseHeaders.end(); it++) {
        response += it->first;
        response += ": ";
        response += it->second;
        response += "\r\n";
    }
    response += "\r\n";
    response += responseBody;
    return response;
}
