//
// Created by Alexis Lafrance on 2/28/22.
//

#include "webserv.hpp"

string readHeader(int fd) {
	string str_buffer;
	char buffer[2];
	fcntl(fd, F_SETFL, O_NONBLOCK);
	while (str_buffer.find("\r\n\r\n") == std::string::npos && read(fd, buffer, 1) > 0){
		str_buffer += buffer[0];
	}
	cout << "Header :" << endl << str_buffer << endl;
	return (str_buffer);
}

string readBody(int fd, map<string, string> & request_header) {
	string str_buffer;

	char buffer[2];
	for (int i = 0; i < atoi(request_header["Content-Length"].c_str()); ++i) {
		if (read(fd, buffer, 1) == 0)
			return (str_buffer);
		str_buffer += buffer[0];
		bzero(buffer, 1);
	}
	return (str_buffer);
}

string defragment_request_body(string &request_body_chunked) {
	int posChunk, num;
	size_t start = 0;
	string request_body;
	stringstream numbers;
	char base[] = "0123456789";

	while(start < request_body_chunked.length()) {
		// FIND THE NUMBERS TO READ
		posChunk = request_body_chunked.find("\r\n", start);
		string len = request_body_chunked.substr(start, posChunk - start);
		// CONVERT TO DECIMAL
		num = ft_atoi_base(len.c_str(), base);
		// READ JUST THE CHUNKED ELEMENTS
		request_body += request_body_chunked.substr(posChunk + 2, num);
		start +=  num + 5;
	}
	return (request_body);
}
