//
// Created by Codage on 25/01/2022.
//

#include "Response.hpp"
#include "webserv.hpp"

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
	size_t pos = path.find_last_of(".");
	if (pos == string::npos)
		return ("none");
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

void Response::setMapType() {
	ifstream ifs("types.csv");
	string line;
	size_t pos;
	if (ifs) {
		while (getline(ifs, line)) {
			pos = line.find(",");
			types.insert(make_pair(line.substr(0, pos), line.substr(pos + 1, line.length() - (pos + 1))));
		}
	}
}

void Response::setContentType(string path){
	string extension = findExtension(path);
	if (extension == "none"){
		contentType = "text/html\n";
	}
	else {
		cout << path << endl;
		map<string, string>::iterator it;
		it = types.find(extension);
		if (it != types.end()) {
			contentType = it->second + "\n";
		}
		else
			contentType = "text/plain\n";
	}
}

void Response::fillHeader(string file, string &path){
	setContentType(path);
	contentLength =  to_string(file.length());
	header = "HTTP/1.1 " + status + "Content-Type: " + contentType + "Content-Length: " + contentLength + "\n\n";
	body = file;
	response = header + body;
	length = header.length() + body.length();
	cout << header << endl;
}

