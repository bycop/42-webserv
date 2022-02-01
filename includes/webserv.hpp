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
#include <cstdlib>
#include "socket.hpp"
#include "create_html.hpp"
#include "send_page.hpp"

#define PORT 8080
using namespace std;

// PARSER_REQUEST.CPP
map<string, string> parsing_request_header(int fd);
string parsing_request_body(int fd, map<string, string> const& request_header);

// BACKEND.CPP
string backend_page(map<string, string> & request_header, string & request_body);

#endif //WEBSERV_WEBSERV_HPP
