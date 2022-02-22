//
// Created by Quentin Robert de beauchamp on 2/8/22.
//

#include "webserv.hpp"

void ft_error(const char *err) {
	perror(err);
	exit(EXIT_FAILURE);
}

int 	checkError(std::string &path, Response &response, Data &data, std::map<std::string, std::string> request_header){
	std::ifstream ifs(path);
	(void)data;
	if (!ifs || path.find("//") != std::string::npos || response.getStatus() != "200 OK\n")
		response.setStatus("404 Not Found");
	else if (request_header["method"] != "GET" && request_header["method"] != "POST" && request_header["method"] != "DELETE")
		response.setStatus("501 Not Implemented");
	else if (request_header["version"] != "HTTP/1.1")
		response.setStatus("505 HTTP Version Not Supported");
	else
		return (0);
	return (1);
}

int	checkTimeOut(time_t start, int timeOut){
	if (time(0) - start > timeOut)
		return (1);
	return (0);
}


