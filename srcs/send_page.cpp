//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

int 	checkError(std::string &path, Response &response, Server &server, std::map<std::string, std::string> request_header){
	std::ifstream ifs(path);
	(void)server;
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
		response.setStatus("200 OK");
        response.fillHeader(file, path);
    }
	else
		response.setStatus("404 Not Found");
}

void display_page(int &new_socket, std::map<std::string, std::string> &request_header, bool autoindex, Response &response, string &request_body, Server &server) {
	string path = "." + request_header["path"];
	DIR *dir;
	if (checkError(path, response, server, request_header))
		create_error_page(response, server);
	else if (path.find(".py") != string::npos)
		response.fillHeaderCGI(backend_page(request_header, request_body));
	else if (autoindex && (dir = opendir(const_cast<char *>(path.c_str()))) != NULL)
		create_indexing_page(dir, request_header["path"], response);
	else
		openFile(path, response);
	write(new_socket, const_cast<char *>(response.getResponse().c_str()), response.getResponse().length());
	response.resetResponse();
}
