//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include<iostream>
#include<fstream>
#include<sstream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <map>

int ft_autoindex(int new_socket, std::string p_index);

#endif //WEBSERV_SOCKET_HPP
