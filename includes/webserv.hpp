//
// Created by Alexis Lafrance on 1/13/22.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP
#include <algorithm>
#include <iostream>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>
#include "../class/Response.hpp"
#include "../class/Data.hpp"
#include <fcntl.h>
#include <sys/event.h>
#include "socket.hpp"
#include "create_page.hpp"
#include "send_page.hpp"
#include <pthread.h>
#include <ctime>
#include <unistd.h>
#define TIMEOUT 5

struct Time{
	bool timeout;
	int  time_sec;
};

#define PORT 8080
using namespace std;

// PARSER_REQUEST.CPP
map<string, string> parsing_request_header(Response &response, string &read_request);
void parsing_request_body(map<string, string> const& request_header, Response &response, string &read_request);
string readRequest(int fd, Response &response);

// BACKEND.CPP
string backend_page(map<string, string> & request_header, string & request_body);

// PARSER_CONF
int parser_conf(Data &data, string const& file_path);

// ERROR
void 	ft_error(const char *err);
int 	checkError(std::string &path, Response &response, Data &data, std::map<std::string, std::string> request_header);
void	checkTimeOutParsing(pair<map<string, string>, string> &request, int &event_fd, Response &response);
int		checkTimeOut(time_t start, int timeOut);

// UTILS
void display_banner();
bool include_in_vector(vector<int> &server_socket, int event_fd);
bool endsWith(const string &str, const string &suffix);
string splitPartsByParts(string const& line, const char delimiter, size_t *start);
bool startsWith(const string &str, const string &prefix);
// KEVENT
void createEvent(int kq, int fd);
void init_kqueue(vector<int> &server_socket, int &kq);
// PROCESS_REQUEST
void create_connection(int event_fd, int kq, Data &data);
void end_connexion(Data &data, int socket_fd);
void process_request(int &fd, Response &response, Data &data);

#endif //WEBSERV_WEBSERV_HPP
