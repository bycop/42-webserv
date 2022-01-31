//
// Created by Codage on 25/01/2022.
//

#include "Response.hpp"

Response::Response() : status("HTTP/1.1 "), contentType("Content-Type: "), contentLength("Content-Length: "), header(status + contentType + contentLength) {
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

string Response::fillHeader(string file, string stat, string content_type){
    status = stat + "\n";
    contentType =  content_type + "\n";
    contentLength =  to_string(file.length());
    header = "HTTP/1.1 " + status + "Content-Type: " + contentType + "Content-Length: " + contentLength + "\n\n";
    return (header);
}

string Response::getHeader() {
    return (header);
}

