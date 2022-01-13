//
// Created by Alexis Lafrance on 1/13/22.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <iostream>
#include <map>
#include<iostream>
#include<fstream>
#include<sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


// PARSING REQUEST
std::multimap<std::string, std::string> parsing_request(int fd);

#endif //WEBSERV_WEBSERV_HPP
