//
// Created by Quentin Robert de beauchamp on 14/01/2022.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

int ft_autoindex(int new_socket, std::map<std::string, std::string> request);
std::string ft_openFile(std::string path);

#endif //WEBSERV_REQUEST_HPP
