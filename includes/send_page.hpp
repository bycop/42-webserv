//
// Created by Quentin Robert de beauchamp on 14/01/2022.
//

#ifndef WEBSERV_SEND_PAGE_HPP
#define WEBSERV_SEND_PAGE_HPP
#include "webserv.hpp"

void display_page(int &new_socket, std::map<std::string, std::string> &request_header, Response &response, string &request_body, Server &server, Location &location);
bool openFile(std::string path, Response &response);

#endif //WEBSERV_SEND_PAGE_HPP
