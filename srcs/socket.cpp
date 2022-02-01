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

void receiving_information(int &server_socket, sockaddr_in &address){
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
		cout << "ACCEPT DONE" << endl;
		request_header = parsing_request_header(new_socket);
		cout << "PARSING REQUEST DONE" << endl;
		request_body = parsing_request_body(new_socket, request_header);
		cout << "PARSING REQUEST BODY DONE" << endl;
		if (request_header["path"].find(".py") < request_header["path"].length()) {
			html_content = backend_page(request_header, request_body);
			std::string content = ft_header(html_content.length(), "200", "text/html") + html_content;
			write(new_socket, content.c_str(), content.length());
		}
		else
			display_page(new_socket, request_header, true);
		cout << "DISPLAY PAGE DONE" << endl;
		close(new_socket);
	}
	close(server_socket);
}
