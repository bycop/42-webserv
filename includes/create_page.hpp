//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#ifndef WEBSERV_CREATE_HTML_HPP
#define WEBSERV_CREATE_HTML_HPP
#include <dirent.h>
#include "webserv.hpp"
#include "../class/Response.hpp"

void create_indexing_page(DIR *dir, std::string path, Response &response);
void create_error_page(Response &response, Server &server);

#endif //WEBSERV_CREATE_HTML_HPP
