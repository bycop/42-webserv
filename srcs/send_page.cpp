//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

int 	checkError(std::string &path, Response &response, Server &server, std::map<std::string, std::string> request_header){
	std::ifstream ifs(path);
	(void)server;
	if (response.getStatus() != "200 OK\n" && !response.getStatus().empty())
		return (1);
	if (!ifs || path.find("//") != std::string::npos)
		response.setStatus("404 Not Found");
	else if (request_header["method"] != "GET" && request_header["method"] != "POST" && request_header["method"] != "DELETE")
		response.setStatus("501 Not Implemented");
	else if (request_header["version"] != "HTTP/1.1")
		response.setStatus("505 HTTP Version Not Supported");
	else if (response.getStatus() == "200 OK\n")
		return (0);
	return (1);
}

void openFile(std::string path, Response &response){
    std::ifstream ifs(path);
    std::string page;
    if (ifs) {
        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string file = oss.str();
		response.fillBody(file);
		response.setStatus("200 OK");
    }
	else
		response.setStatus("404 Not Found");
}


void add_slash_to_directory(string & path) {
	if(opendir(const_cast<char *>((path).c_str())) != NULL && path[path.length() - 1] != '/')
		path.insert(path.length(), "/");
}

void display_page(int &new_socket, std::map<std::string, std::string> &request_header, Response &response, string &request_body, Server &server, Location &location) {
	DIR *dir;
	string pathModify = "." + request_header["path"];

	add_slash_to_directory(pathModify);
	if (endsWith(pathModify, ".py"))
		response.responseCGI(backend_page(request_header, request_body, location, server), request_header);
	else {
		if (checkError(pathModify, response, server, request_header))
			create_error_page(response, server);
		else if (!server.isAutoindex() && (dir = opendir(const_cast<char *>(pathModify.c_str()))) != NULL)
			create_indexing_page(dir, pathModify, response);
		else
			openFile(pathModify, response);
		response.fillHeader(pathModify, request_header, false);
	}
	response.response_http(new_socket);
	response.resetResponse();
}
