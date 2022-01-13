//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "socket.hpp"

int ft_autoindex(int new_socket, std::string file){
	return (write(new_socket, const_cast<char *>(file.c_str()), strlen(file.c_str())));
}

