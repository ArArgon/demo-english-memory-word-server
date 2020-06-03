#include "controller.h"

#include <iostream>

#include "service.h"
std::string dealing(ParseHttpRequest* request){
    std::string res;
    if (request->getRequestMethod() == "GET" && request->getRequestPath() == "/translate"){
        std::string query = request->getRequestQuery("query");
        MakeHttpResponse* http = new MakeHttpResponse();
        std::string serviceResult = translate(query);
        if (serviceResult == "error") {
            std::string errMsg = "An error occured in the server.";
            res = http->setResponseHeader("Content-Type", "text/plain")->setCode(500)->setResponseBody(errMsg)->generateHttpResponse();
        } else {
            res = http->setResponseHeader("Content-Type", "text/html;charset=utf-8")->setCode(200)->setResponseBody(translate(query))->generateHttpResponse();
        }
    } else if (request->getRequestMethod() == "GET" && request->getRequestPath() == "/getProblem"){
        std::string id = request->getRequestQuery("id");
        std::string difficulty = request->getRequestQuery("difficulty");
        MakeHttpResponse* http = new MakeHttpResponse();
        res = http->setResponseHeader("Content-Type", "text/html;charset=utf-8")->setCode(200)->setResponseBody(getProblem(id, difficulty))->generateHttpResponse();
    } else if (request->getRequestMethod() == "POST" && request->getRequestPath() == "/submit"){

    } else {
        MakeHttpResponse* http = new MakeHttpResponse();
        std::string errMsg = "The requested file was not found on this server.";
        res = http->setResponseHeader("Content-Type", "text/plain")->setCode(404)->setResponseBody(errMsg)->generateHttpResponse();
    }
    return res;
}
