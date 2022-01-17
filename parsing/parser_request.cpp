//
// Created by Alexis Lafrance on 1/13/22.
//

#include "webserv.hpp"
using namespace std;

pair<string,int> readLine(FILE *file) {
	size_t ret_line, len;
	char *tmp = NULL;
	ret_line = getline(&tmp, &len, file);
	string line(tmp);
	return (make_pair(line, ret_line));
}

std::string splitPartsByParts(string const& line) {
	static int start = 0, end = 0;
	string part;

	end = line.find(" ", start);
	part = line.substr(start, end - start);
	start = end + 1;
	return (part);
}

void parse_first_line_request(string const& line, multimap<string, string> &request) {
	string part;

	cout << line;
	request.insert(make_pair("method", splitPartsByParts(line)));
	request.insert(make_pair("path", splitPartsByParts(line)));
	request.insert(make_pair("version", splitPartsByParts(line))); // AND DONT TAKE TWO LAST CHARS
}

multimap<string, string> parsing_request(int fd) {
	multimap<string, string> request;
	char buffer[30000] = {0};

	read(fd, buffer, 30000);
	std::istringstream is(buffer);
	std::string line;
	while(std::getline(is, line))
		cout << line << std::endl;

//	pair<string, int> line = readLine(file);
//	int del;

//	parse_first_line_request(line.first, request);
//	while(line.second != 2 && line.first != "\n\n") { 	// THE LAST LINE OF THE HEADER (BEFORE THE BODY)
//		line = readLine(file);
//		del = line.first.find(":");
//		cout << line.first;
//		cout << "SUB : " << line.first.substr(del + 2, line.first.length() - del - 4) << std::endl;
//		request.insert(make_pair(line.first.substr(0, del), line.first.substr(del + 2, line.first.length() - (del + 2) - 2)));
//	}
	return (request);
}