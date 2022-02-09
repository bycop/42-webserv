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
	cout << "CONTENT-TYPE: " << endl;
	cout << path << endl;
	if (extension.empty()){
		contentType = "text/html";
	}
	else {
		map<string, string>::iterator it;
		it = types.find(extension);
		if (it != types.end()) {
			contentType = it->second;
		}
		else
			contentType = "text/plain";
	}
	header += "Content-Type: " + contentType + '\n';
}


void Response::responseCGI(const string& cgi_content, map<string, string> & request_header) {
	size_t pos_spliter = cgi_content.find("\n\n");
	if (pos_spliter == string::npos) {
		fillHeader(request_header["path"], request_header, true);
		body = cgi_content;
	}
	else {
		body = cgi_content.substr(pos_spliter + 2, cgi_content.length() - pos_spliter);
		fillHeader(request_header["path"], request_header, true);
		header += cgi_content.substr(0, pos_spliter + 2);
	}
}

void Response::fillHeader(string &path, map<string, string> & request_header, bool is_cgi){
	header = "HTTP/1.1 " + status;
	header += "Content-Length: " + std::to_string(body.length()) + '\n';
	if (request_header["Connection"] == "keep-alive")
		header += "Connection: keep-alive\n";
	if (!is_cgi)
		setContentType(path);
}

void	Response::response_http(int new_socket) {
	response = header + '\n' + body;
	cout << "- Response :" << endl;
	cout << header << endl;
//	cout << response << endl; // TODO: TEST
	write(new_socket, const_cast<char *>(response.c_str()), response.length());
}

void Response::fillBody(string const& content) {
	body = content;
}
