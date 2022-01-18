//
// Created by Alexis Lafrance on 1/13/22.
//

#include "webserv.hpp"
using namespace std;

string splitPartsByParts(string first_line) {
	static int start = 0, end = 0;
	string part;

	end = first_line.find(" ", start);
	part = first_line.substr(start, end - start);
	start = end + 1;
	return (part);
}

void parse_first_line_request(std::istringstream & is, multimap<string, string> &request) {
	string part;
	string first_line;

	getline(is, first_line);
	cout << first_line << endl; // TODO: TEST
	request.insert(make_pair("method", splitPartsByParts(first_line)));
	request.insert(make_pair("path", splitPartsByParts(first_line)));
	request.insert(make_pair("version", splitPartsByParts(first_line))); // AND DONT TAKE TWO LAST CHARS
}

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

string readBody(int fd, multimap<string, string> const& request) {
	string body;
	char buffer[2];
	int length = std::atoi(request.find("Content-Length")->second.c_str());
	for (int i = 0; i < length; ++i) {
		read(fd, buffer, 1);
		body += buffer[0];
	}
	return (body);
}

multimap<string, string> parsing_request(int fd) {
	multimap<string, string> request;
	std::istringstream is(readHeader(fd)); // READ THE HEADER (NOT THE BODY
	std::string line;
	size_t pos_del;
	parse_first_line_request(is, request);
	while(std::getline(is, line)) {
		pos_del = line.find(":");
		cout << line << endl; // TODO: TEST
		request.insert(make_pair(line.substr(0, pos_del), // KEY
								 	line.substr(pos_del + 2, line.length() - (pos_del + 2) - 1))); // VALUE WITHOUT : AND \n\r
	}
	if (request.find("method")->second == "POST")
		cout << readBody(fd, request) << endl;
	return (request);
}
