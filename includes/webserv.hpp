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
#include <fcntl.h>
#include <sys/event.h>
#include "socket.hpp"
#include "create_page.hpp"
#include "send_page.hpp"
#include "Response.hpp"
#include "../class/Data.hpp"

#define PORT 8080
using namespace std;

// PARSER_REQUEST.CPP
map<string, string> parsing_request_header(int fd, Response &response);
string parsing_request_body(int fd, map<string, string> const& request_header, Response &response);

// BACKEND.CPP
string backend_page(map<string, string> & request_header, string & request_body);

// PARSER_CONF
int parser_conf(Data &data, string const& file_path);

// ERROR
void ft_error(const char *err);
// UTILS
void display_banner();
bool include_in_vector(vector<int> &server_socket, int event_fd);
// KEVENT
void createEvent(int kq, int fd);
void init_kqueue(vector<int> &server_socket, int &kq);
// PROCESS_REQUEST
void create_connection(int event_fd, int kq, Data &data);
void end_connexion(Data &data, int socket_fd);
void process_request(int &fd, map<string, string> &request_header, string &request_body, Response &response, Data &data);
#endif //WEBSERV_WEBSERV_HPP
