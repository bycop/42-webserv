//
// Created by Alexis Lafrancette on 1/13/22.
//

#include "webserv.hpp"

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

string get_path_info_and_del_to_path(string &path) {
	string path_info;
	size_t pos_split_path = path.find('/', path.find("."));
	if (path.find_last_of('.') < pos_split_path && pos_split_path != string::npos) {
		path_info = path.substr(pos_split_path, path.length() - pos_split_path); // GET PATH INFO / INTO THE END
		path = path.substr(0, pos_split_path); // GET PATH TRANSLATED BEGINNING INTO THE /
	}
	return(path_info);
}

char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

string url_decode(string &text) {
	char h;
	ostringstream escaped;
	escaped.fill('0');

	for (string::iterator i = text.begin(); i != text.end(); ++i) {
		string::value_type c = (*i);
		if (c == '%') {
			if (i[1] && i[2]) {
				h = from_hex(i[1]) << 4 | from_hex(i[2]);
				escaped << h;
				i += 2;
			}
		} else if (c == '+')
			escaped << ' ';
		else
			escaped << c;
	}
	return escaped.str();
}

// FIRST_LINE
void parse_first_line_request(std::istringstream &is, map<string, string> &request, Response &response) {
	string part, first_line;
	size_t start = 0;
	std::pair<string, string> path_info_translated;
	getline(is, first_line);
	if (first_line.empty())
		return ;
	if (first_line.size() > MAX_SIZE_URI){
		response.setStatus("414 Request-URI Too Long");
		return ;
	}
	if (first_line.find("  ") != string::npos)
			response.setStatus("400 Bad Request");
	request.insert(make_pair("method", splitPartsByParts(first_line, ' ', &start)));
	request.insert(make_pair("path", splitPartsByParts(first_line, ' ', &start)));
	request.insert(make_pair("query", parse_query_string(request["path"])));
	request.insert(make_pair("version", splitPartsByParts(first_line, ' ', &start)));
	if (!request["version"].empty())
		request["version"].erase(request["version"].length() - 1, 2);
	if (!request["path"].empty())
		request["path"] = url_decode(request["path"]);
	if (!startsWith(request["path"], "/"))
		response.setStatus("400 Bad Request");
}

// THE MAIN FCT OF THE PARSING
map<string, string> parsing_request_header(Response &response, string read_request) {
	map<string, string> request_header;
	std::istringstream is(read_request);
	std::string line;
	size_t pos_del;

	parse_first_line_request(is, request_header, response);
	// PARSING HEADER
	while(std::getline(is, line) && !line.empty()) {
		pos_del = line.find(':');
		if (pos_del == 0 || (pos_del == string::npos && line.length() < 1)) {
			response.setStatus("400 Bad Request");
			break;
		}
		removeWS(line, pos_del);
		if (request_header.find(line.substr(0, pos_del)) != request_header.end() || line.substr(0, pos_del).find(' ') != string::npos)
			response.setStatus("400 Bad Request");
		request_header.insert(make_pair(line.substr(0, pos_del), // KEY
								 	line.substr(pos_del + 1, line.length() - (pos_del + 2)))); // VALUE WITHOUT : \n\r
	}
	pos_del = read_request.find("\r\n\r\n");
	if (pos_del != string::npos)
		read_request.erase(0, read_request.find("\r\n\r\n") + 4);
	return (request_header);
}