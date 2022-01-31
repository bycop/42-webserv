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

std::string parse_query_string(std::string & path) {
	std::string query;
	std::string path_tmp = path;
	if (path.find('?') < path.length()) {
		size_t start = 0;
		path = splitPartsByParts(path_tmp, '?', &start);
		query = splitPartsByParts(path_tmp, '?', &start);
	}
	return (query);
}

std::pair<string, string> get_path_info_and_translated(string const& path) {
	std::pair<string, string> path_info_and_translated;
	size_t pos_split_path = path.rfind('/');
	if (path.find('.') < pos_split_path) {
		path_info_and_translated.first = path.substr(pos_split_path, path.length() - pos_split_path); // GET PATH INFO / INTO THE END
		path_info_and_translated.second = path.substr(0, pos_split_path); // GET PATH TRANSLATED BEGINNING INTO THE /
	}
	return(path_info_and_translated);
}

// FIRST_LINE
void parse_first_line_request(std::istringstream &is, map<string, string> &request) {
	string part, first_line;
	size_t start = 0;
	std::pair<string, string> path_info_translated;
	getline(is, first_line);
	cout << first_line << endl; // TODO: TEST
	request.insert(make_pair("method", splitPartsByParts(first_line, ' ', &start)));
	request.insert(make_pair("path", splitPartsByParts(first_line, ' ', &start)));
	path_info_translated = get_path_info_and_translated(request["path"]);
	request.insert(make_pair("path_info", path_info_translated.first));
	request.insert(make_pair("path_translated", path_info_translated.second));
	request.insert(make_pair("query", parse_query_string(request["path"])));
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

string read_body_chunk(int fd) {
	string body, len_buffer;
	int len = -1;
	char buffer[2];
	bzero(buffer, 2);
	while (len != 0) {
//		cout << "Buffer len: ";
		while(len_buffer.find("\r\n") > len_buffer.length()) {
			read(fd, buffer, 1);
//			cout << buffer;
			len_buffer += buffer[0];
		}
//		cout << endl;
		len = atoi(len_buffer.c_str());
		len_buffer.clear();
		if (len < 0) {
			cerr << "Negative chunk's len's" << endl;
			return (body);
		}
//		cout << "body read : ";
		for (int i = 0; i < len; i++) {
			read(fd, buffer, 1);
//			cout << buffer;
			body += buffer[0];
		}
//		cout << endl;
		for (int i = 0; i < 2; ++i) { // READ THE \r\n
			read(fd, buffer, 1);
//			cout << "skip" << endl;
		}
	}
	return (body);
}


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

string parsing_request_body(int fd, map<string, string> const& request_header) {
	string request_body;

	if (request_header.find("method")->second == "POST") {
		if (request_header.find("Transfer-Encoding") != request_header.end() &&
			request_header.find("Transfer-Encoding")->second == "chunked") // TRANSFER ENCODING : CHUNKED
			request_body = read_body_chunk(fd);
		else if (request_header.find("Content-Length") != request_header.end()) { // CLASSIC WAY TO READ BODY
			int length = stoi(request_header.find("Content-Length")->second);
			request_body = readBody(fd, length);
		} else
			cerr << "Impossible to parse POST" << endl;
	}
	cout << "BODY : " << request_body << endl;
	return (request_body);
}
