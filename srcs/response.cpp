//
// Created by Codage on 25/01/2022.
//

#include "Response.hpp"

Response::Response() : status("HTTP/1.1 "), contentType("Content-Type: "), contentLength("Content-Length: "), header(status + contentType + contentLength) {
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

void 	Response::setMapType() {

}

void Response::setContentType(string extension){
	if (extension == "html")
		contentType = "text/html\n";
	if (extension == "png" || extension == "jpg")
		contentType = "image/png\n";
}

string Response::getResponse() {
	return (header + body);
}

size_t Response::getLength(){
	return (header.length() + body.length());
}

void Response::fillHeader(string file, string extension){
	setContentType(extension);
    contentLength =  to_string(file.length());
    header = "HTTP/1.1 " + status + "Content-Type: " + contentType + "Content-Length: " + contentLength + "\n\n";
	body = file;
}

string Response::getHeader() {
    return (header);
}

string Response::getBody() {
	return (body);
}

void Response::setBody(string file) {
	body = file;
}

void		Response::setStatus(string stat) {
	status = stat + "\n";
}
string		Response::getStatus(void) {
	return (status);
}

