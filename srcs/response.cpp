//
// Created by Codage on 25/01/2022.
//

#include "Response.hpp"

///COPLIEN FORM
Response::Response(){
	setMapType();
}

Response::~Response() {
}

Response::Response(const Response &cpy) {
    *this = cpy;
}

Response &Response::operator=(const Response &cpy) {
    status = cpy.status;
    contentType = cpy.contentType;
    contentLength = cpy.contentLength;
    header = cpy.header;
    return (*this);
}

string Response::findExtension(string path) {
	return (path.substr(path.find_last_of(".") + 1));
}


/// GETTERS
string		Response::getStatus(void) {
	return (status);
}

string Response::getHeader() {
    return (header);
}

string Response::getBody() {
	return (body);
}

string Response::getResponse() {
	return (response);
}

size_t Response::getLength(){
	return (length);
}


///SETTERS
void Response::setStatus(string stat) {
	status = stat + "\n";
}

void Response::setContentType(string path){
	cout << findExtension(path) << endl;
}

void Response::setMapType() {

}

void Response::fillHeader(string file, string path){
	setContentType(path);
	contentLength =  to_string(file.length());
	header = "HTTP/1.1 " + status +/* "Content-Type: " + contentType + "Content-Length: " + contentLength + "\n\n"*/ + "\n\n";
	body = file;
	response = header + body;
	length = header.length() + body.length();
	cout << header << endl;
}


