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
	Error *error;
	error = static_cast<Error *>(arg);
	error->request->first = parsing_request_header(*(error->event_fd), *(error->response));
	error->request->second = parsing_request_body(*(error->event_fd), error->request->first, *(error->response));
	error->parsed = true;
	return (error);
}

void	checkTimeOutParsing(pair<map<string, string>, string> &request, int &event_fd, Response &response){
	pthread_t thread;
	Error *error;
	error = static_cast<Error *>(malloc(sizeof(Error)));

	error->event_fd = event_fd;
	error->response = response;
	error->request = request;
//	error->parsed = false;
//	time_t now = time(0);
	pthread_create(&thread, NULL, &launchParsing, error);
//	while (!error->parsed){
//		if (time(0) > now + 5) {
//			pthread_cancel(thread);
//			response->setStatus("408 Request Timeout");
//		}
//	}
//	request = error->request;
//	event_fd = error->event_fd;
//	response = error->response;
	free(error);
}
