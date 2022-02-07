//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include "../class/Server.hpp"
#include "../class/Data.hpp"
#include "../class/Response.hpp"

void create_socket(vector<int> &server_socket, vector<Server> &servers);
void receiving_information(vector<int> &server_socket, Response &response, Data& data);

#endif //WEBSERV_SOCKET_HPP
