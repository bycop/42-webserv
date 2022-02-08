//
// Created by Codage on 25/01/2022.
//

#include "Response.hpp"
#include "webserv.hpp"

///COPLIEN FORM
Response::Response() :  status("200 OK\n"), contentType("text/plain\n"){
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
	size_t pos = path.find_last_of('.');
	if (path.find_last_of('/') == path.length() - 1)
		return (string());
	return (path.substr(pos + 1));
}

/// GETTERS
string		Response::getStatus() {
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
void Response::setStatus(const string& stat) {
	status = stat + "\n";
}

void Response::setMapType() {
	ifstream ifs("types.csv");
	string line;
	size_t pos;
	if (ifs) {
		while (getline(ifs, line)) {
			pos = line.find(',');
			types.insert(make_pair(line.substr(0, pos), line.substr(pos + 1, line.length() - (pos + 1))));
		}
	}
}

void Response::setContentType(string &path){
	string extension = findExtension(path);
	if (extension.empty()){
		contentType = "text/html\n";
	}
	else {
		map<string, string>::iterator it;
		it = types.find(extension);
		if (it != types.end()) {
			contentType = it->second + "\n";
		}
		else
			contentType = "text/plain\n";
	}
}

void Response::fillHeaderCGI(const string& content, map<string, string> request_header) {
	header = "HTTP/1.1 " + status;
	if (request_header["Connection"] == "keep-alive")
		header += "\nConnection: keep-alive\n\n";
	header += "Content-Length:" + to_string(content.length()) + '\n';
	response = header + content;
	cout << response << endl; // TODO: TEST
}

void Response::fillHeader(string file, string &path, map<string, string> request_header){
	setContentType(path);
	contentLength =  to_string(file.length());
	header = "HTTP/1.1 " + status;
	header += "Content-Type: " + contentType + "Content-Length: " + contentLength;
	if (request_header["Connection"] == "keep-alive")
		header += "\nConnection: keep-alive\n\n";
	response = header + file;
	cout << "- Header response :" << endl;
	cout << header << endl;
}
