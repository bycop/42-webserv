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
using namespace std;
// PARSING REQUEST
std::pair<multimap<string, string>, map<string, string> > parsing_request(int fd);
#endif //WEBSERV_WEBSERV_HPP
