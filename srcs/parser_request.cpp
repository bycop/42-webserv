//
// Created by Alexis Lafrance on 1/13/22.
//

#include "webserv.hpp"

// UTILITY
string splitPartsByParts(string const& line, const char delimiter, size_t *start) {
	size_t end;
	string part;

	if (*start > line.length())
		return (part);
	end = line.find(delimiter, *start);
	if (end == string::npos) {
		part = line.substr(*start, line.length() - *start);
		*start = line.length() + 1;
		return (part);
	}
	part = line.substr(*start, end - *start);
	*start = end + 1;
	return (part);
}

// FIRST_LINE
void parse_first_line_request(std::istringstream &is, map<string, string> &request) {
	string part, first_line;
	size_t start = 0;

	getline(is, first_line);
	cout << first_line << endl; // TODO: TEST
	request.insert(make_pair("method", splitPartsByParts(first_line, ' ', &start)));
	request.insert(make_pair("path", splitPartsByParts(first_line, ' ', &start)));
	request.insert(make_pair("version", splitPartsByParts(first_line, ' ', &start))); // TODO:  DONT TAKE TWO LAST CHARS
}

// MY READER YEAAAAH
string readHeader(int fd) {
	string str_buffer;
	string line;
	char buffer[2];
	while (str_buffer.find("\n\r\n") == std::string::npos) {
		read(fd, buffer, 1);
		str_buffer += buffer[0];
	}
	return (str_buffer);
}

string readBody(int fd, int length) {
	string body;
	char buffer[2];
	for (int i = 0; i < length; ++i) {
		read(fd, buffer, 1);
		body += buffer[0];
	}
	return (body);
}


// PARSING BODY (FOR NOW WHITOUT THE CGI I DON'T KNOW IF I NEED TO PARSE THE BODY)
//void cut_content_type_multipart(map<string, string> &request) {
//	if (request.find("Content-Type") == request.end())
//		return ;
//	size_t pos = 0;
//	string &content = request.find("Content-Type")->second, content2;
//	if (content.find("multipart/form-data") != string::npos){
//		splitPartsByParts(content, ';', &pos);
//		content2 = splitPartsByParts(content, ';', &pos);
//		content2.erase(0, 1); // ERASE FIRST WHITESPACE
//		pos = content2.find('=') + 1;
//		request.insert(make_pair("boundary", splitPartsByParts(content2, '=',&pos)));
//		content = "multipart/form-data"; // Delete boundary to content
//	}
//}

// THE MAIN FCT OF THE PARSING
map<string, string> parsing_request_header(int fd) {
	map<string, string> request_header;
	std::istringstream is(readHeader(fd));
	std::string line, body;
	size_t pos_del;

	parse_first_line_request(is, request_header);

	// PARSING HEADER
	while(std::getline(is, line)) {
		pos_del = line.find(':');
		cout << line << endl; // TODO: TEST
		request_header.insert(make_pair(line.substr(0, pos_del), // KEY
								 	line.substr(pos_del + 2, line.length() - (pos_del + 2) - 1))); // VALUE WITHOUT : AND \n\r
	}
	return (request_header);
}

// PARSING BODY
string parsing_request_body(int fd, map<string, string> const& request_header) {
	string request_body;

	if (request_header.find("method")->second == "POST") {
		if (request_header.find("Content-Length") == request_header.end()) {
			cerr << "Impossible to parse POST" << endl;
		} else {
			int length = stoi(request_header.find("Content-Length")->second);
			request_body = readBody(fd, length);
		}
	}
	return (request_body);
}
