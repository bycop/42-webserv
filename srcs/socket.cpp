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
	map<string, string> request_header;
	string request_body;
	string html_content;

	if (listen(server_socket, 10) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while (1) { //TODO: CHOOSE A WAY TO STOP OUR SERVER DIFFERENTLY
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_socket, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		request_header = parsing_request_header(new_socket);
		request_body = parsing_request_body(new_socket, request_header);
		display_page(new_socket, request_header, true, response, request_body);
		close(new_socket);
	}
	close(server_socket);
}
