//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#ifndef WEBSERV_CREATE_HTML_HPP
#define WEBSERV_CREATE_HTML_HPP
#include <dirent.h>
#include "webserv.hpp"

void create_indexing_page(DIR *dir, std::string &path, Response &response, Location &location);
void create_error_page(Response &response, Server &server, string html_content = std::string());
#endif //WEBSERV_CREATE_HTML_HPP
