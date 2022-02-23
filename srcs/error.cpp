//
// Created by Quentin Robert de beauchamp on 2/8/22.
//

#include "webserv.hpp"

void ft_error(const char *err) {
	perror(err);
	exit(EXIT_FAILURE);
}

int	checkTimeOut(time_t start, int timeOut){
	if (time(0) - start > timeOut)
		return (1);
	return (0);
}


