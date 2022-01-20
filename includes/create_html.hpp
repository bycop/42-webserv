//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#ifndef WEBSERV_CREATE_HTML_HPP
#define WEBSERV_CREATE_HTML_HPP
#include <dirent.h>

std::string create_indexing_page(DIR *dir, std::string path);
std::string	create_existing_page(std::string &path, std::string &status);

#endif //WEBSERV_CREATE_HTML_HPP
