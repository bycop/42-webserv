//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#ifndef WEBSERV_CREATE_HTML_HPP
#define WEBSERV_CREATE_HTML_HPP
#include <dirent.h>

void create_indexing_page(DIR *dir, std::string path, Response &response);
void create_error_page(std::string &path, Response &response);

#endif //WEBSERV_CREATE_HTML_HPP
