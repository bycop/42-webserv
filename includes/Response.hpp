//
// Created by Codage on 25/01/2022.
//

#ifndef WEBSERV_HEADERRESPONSE_H
#define WEBSERV_HEADERRESPONSE_H
#include <iostream>
#include <map>

using namespace std;

class Response {
private:
    string  status;
    string  contentType;
	map<string, string> type;
    string  contentLength;
    string  header;
    string  body;
	void setMapType(void);
public:
    Response();
    ~Response();
    Response(Response const &cpy);
    Response &operator=(Response const &cpy);
    string getHeader();
	string getBody();
	void 	setStatus(string);
	string	getStatus(void);
	size_t getLength();
	string getResponse();
	void setBody(string);
    void fillHeader(string file, string content_type);
	void setContentType(string extension);
//	string getContentType(void);
};

#endif //WEBSERV_HEADERRESPONSE_H
