//
// Created by Codage on 25/01/2022.
//

#ifndef WEBSERV_HEADERRESPONSE_H
#define WEBSERV_HEADERRESPONSE_H
#include <iostream>

using namespace std;

class Response {
private:
    string  status;
    string  contentType;
    string  contentLength;
    string  header;
    char    *file;
public:
    Response();
    ~Response();
    Response(Response const &cpy);
    Response &operator=(Response const &cpy);
    string getHeader();
    string fillHeader(string file, string status, string content_type);
};

#endif //WEBSERV_HEADERRESPONSE_H