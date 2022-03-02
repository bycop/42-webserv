//
// Created by Codage on 25/01/2022.
//

#include "webserv.hpp"
#include "../class/Response.hpp"

///COPLIEN FORM
Response::Response() : status("200 OK\n"), contentType("text/plain\n") {
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

string Response::findExtension(string &path) {
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

///SETTERS
void Response::setStatus(const string& stat) {
	if (status == "200 OK\n")
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
	if (extension.empty() || status != "200 OK\n"){
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


// CGI
void Response::responseCGI(const string& cgi_content, map<string, string> & request_header, Server &server) {
	size_t pos_spliter;
	if (endsWith(request_header["path"], ".php"))
		pos_spliter = cgi_content.find("\r\n\r\n");
	else
		pos_spliter = cgi_content.find("\n\n");

	if (pos_spliter == string::npos) {
		responseError( "502 Bad Gateway", server, request_header, cgi_content);
		return ;
	}
	string header_cgi = cgi_content.substr(0, pos_spliter);
	map<string, string> request_header_cgi = parsing_request_header_cgi(header_cgi);
	if (request_header_cgi.find("Content-type") == request_header_cgi.end() && request_header_cgi.find("Content-Type") == request_header_cgi.end() ) {
		responseError("502 Bad Gateway", server, request_header, "Error. We need Content-Type");
		return;
	}
	fillBody(cgi_content.substr(pos_spliter + 2, cgi_content.length() - pos_spliter));
	fillHeader(request_header["path"], request_header, true);
	addHeaderCgi(request_header_cgi);
}

void Response::addHeaderCgi(map<string, string> &request_header_cgi) {
	map<string, string>::iterator it_content_length = request_header_cgi.find("Content-Length");
	if (it_content_length != request_header_cgi.end() && atoi(it_content_length->second.c_str()) <= 0)
		request_header_cgi.erase(it_content_length);
	for (map<string, string>::iterator it = request_header_cgi.begin(); it != request_header_cgi.end(); ++it) {
		if (request_header_cgi.find("Connection") == request_header_cgi.end())
			header += it->first + ": " + it->second + '\n';
	}
	if (it_content_length == request_header_cgi.end())
		header += "Content-Length: " + atoiString(static_cast<int>(body.length())) + '\n';
}

void Response::responseError(const char * error_msg, Server &server, map<string, string> &request_header, string html_content) {
	setStatus(error_msg);
	create_error_page(*this, server, html_content);
	fillHeader(request_header["path"], request_header, false);
}

map<string, string> Response::parsing_request_header_cgi(const string &header_cgi) {
	std::istringstream is(header_cgi);
	map<string, string> request_header_cgi;
	string line;
	size_t pos_del;

	while(std::getline(is, line) && !line.empty()) {
		pos_del = line.find(':');
		string key = line.substr(0, pos_del);
		// ALREADY IN THE HEADER REPLACE IT
		map<string, string>::iterator it;

		if ((it = request_header_cgi.find(key)) != request_header_cgi.end())
			request_header_cgi.erase(it);
		// INSERT KEY, VALUE
		request_header_cgi.insert(make_pair(key, line.substr(pos_del + 2, line.length() - (pos_del + 2))));
	}
	return (request_header_cgi);
}


// RESPONSE HTTP (HEADER + BODY)
void Response::fillHeader(string &path, map<string, string> & request_header, bool is_cgi) {
	header = "HTTP/1.1 " + status;
	if (!is_cgi) {
		header += "Content-Length: " + atoiString(static_cast<int>(body.length())) + '\n';
		setContentType(path);
	}
	if (request_header["Connection"] == "keep-alive")
		header += "Connection: keep-alive\n";
}

void	Response::response_http(int new_socket) {
	response = header + '\n' + body;
	write(new_socket, const_cast<char *>(response.c_str()), response.length());
}

void Response::fillBody(string const& content) {
	body = content;
}

void 	Response::resetResponse(){
	status = "200 OK\n";
	contentType = "text/plain";
	contentLength = string();
	header = string();
	body = string();
	response = string();
}
