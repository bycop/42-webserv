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

void 	*launchParsing(void *arg){
	Error *error = static_cast<Error *>(arg);
	error->_request.first = parsing_request_header(error->_event_fd, error->_response);
	error->_request.second = parsing_request_body(error->_event_fd, error->_request.first, error->_response);
	error->parsed = true;
	return (NULL);
}

void	checkTimeOutParsing(pair<map<string, string>, string> &request, int &event_fd, Response &response){
	pthread_t thread;
	Error *error;
	error = new Error(request, event_fd, response);
	error->parsed = false;
	time_t now = time(0);
	pthread_create(&thread, NULL, &launchParsing, error);
//	pthread_join(thread, &i);
	usleep(1000);
	while (!error->parsed) {
	cout << time(0) - now << endl;
		if (time(0) > now + 5) {
			cout << "quit quit" << endl;
//			pthread_cancel(thread);
			response.setStatus("408 Request Timeout");
			break;
		}
	}
}
