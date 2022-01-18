//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "global.hpp"

std::map<std::string, std::string> ft_request(int new_socket){
	char buffer[30000] = {0};
    long valread;
    valread = read(new_socket, buffer, 30000);
    std::string line;
    std::istringstream is(buffer);
    std::getline(is, line);
    std::map<std::string, std::string> request;
    request[line.substr(0, line.find_first_of(" "))] = line.substr(line.find_first_of(" ") + 1, line.find_last_of(' ') - (line.find_first_of(" ") + 1));
    std::cout << buffer << std::endl;
    return (request);
}

int ft_createSocket(void) {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int enable = 1;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}
    address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("In bind");
		exit(EXIT_FAILURE);
	}
    if (listen(server_fd, 10) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while (1) {
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
			perror("In accept");
			exit(EXIT_FAILURE);
		}
        ft_autoindex(new_socket, ft_request(new_socket), 1);
		close(new_socket);
	}
}
