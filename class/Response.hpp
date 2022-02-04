//
// Created by Codage on 25/01/2022.
//

#ifndef WEBSERV_HEADERRESPONSE_H
#define WEBSERV_HEADERRESPONSE_H
#include <iostream>
#include <map>
#include <string>
#include "Data.hpp"

using namespace std;

class Response {
private:
	string	status;
    string	contentType;
    string	contentLength;
    string	header;
    string	body;
	string	response;
	map<string, string>	types;
	void setMapType(void);
public:
    Response();
    ~Response();
    Response(Response const &cpy);
    Response &operator=(Response const &cpy);
	string	getStatus(void);
    string	getHeader();
	string	getBody();
	string	getResponse();
	size_t	getLength();
	void	setStatus(string);
	void	setContentType(string path);
	void 	fillHeaderCGI(void);
    void	fillHeader(string file, string &path);
	string	findExtension(string path);
	void	resetResponse();
};

#endif //WEBSERV_HEADERRESPOc NSE_H