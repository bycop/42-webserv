//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

void create_socket(int &server_socket, sockaddr_in &address) {
	int enable = 1;

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}
    address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");
	if (bind(server_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("In bind");
		exit(EXIT_FAILURE);
	}
}

void receiving_information(int &server_socket, sockaddr_in &address, Response &response){
	int new_socket;
	int addrlen = sizeof(address);
	std::pair<map<string, string>, string> request;

	if (listen(server_socket, 10) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while (1) {
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_socket, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		request = parsing_request(new_socket);
		display_page(new_socket, request.first, 1, response);
		close(new_socket);
	}
}
