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
#include "socket.hpp"
#include "create_page.hpp"
#include "send_page.hpp"

#define PORT 8080
using namespace std;

// PARSER_REQUEST.CPP
map<string, string> parsing_request_header(int fd, Response &response);
string parsing_request_body(int fd, map<string, string> const& request_header, Response &response);

// BACKEND.CPP
string backend_page(map<string, string> & request_header, string & request_body);

// PARSER_CONF
int parser_conf(Data &data, string const& file_path);
#endif //WEBSERV_WEBSERV_HPP
