//
// Created by Alexis Lafrance on 1/13/22.
//

#include "webserv.hpp"

std::multimap<std::string, std::string> parsing_request(int fd) {
	std::multimap<std::string, std::string> request;
	char *tmp = NULL;
	size_t len = 0;
	size_t ret_line = 0;
	FILE * file_request = fdopen(fd, "r");
	while(ret_line != 2 && (ret_line = getline(&tmp, &len,file_request)) ) {
		std::cout << tmp << "|" << ret_line << std::endl;
	}
	std::cout << "END OF GETLINE" << std::endl;
//	std::string line(tmp);
	return (request);
}