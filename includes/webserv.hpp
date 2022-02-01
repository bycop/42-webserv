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
#include "socket.hpp"
#include "create_html.hpp"
#include "send_page.hpp"
#include "../class/Data.hpp"
#include "../class/Location.hpp"
#include "../class/Server.hpp"

#define PORT 8080

using namespace std;

// PARSING REQUEST
pair<map<string, string>, string > parsing_request(int fd);

// PARSER_CONF
int parser_conf(Data &data, string const& file_path);
#endif //WEBSERV_WEBSERV_HPP
