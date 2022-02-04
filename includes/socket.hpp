//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include "../class/Server.hpp"
#include "Response.hpp"

void create_socket(vector<int> &server_socket, sockaddr_in &address, vector<Server> &servers);
void receiving_information(vector<int> &server_socket, sockaddr_in &address, Response &response);

#endif //WEBSERV_SOCKET_HPP
