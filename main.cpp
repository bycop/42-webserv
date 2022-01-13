//
// Created by Sameo Fournion on 1/12/22.
//

// Server side C/C++ program to demonstrate Socket programming

#include "webserv.hpp"

size_t ft_strlen(std::string str) {
	return (str.length());
}

char *ft_toChar(std::string str) {
	char *c = const_cast<char *>(str.c_str());
	return (c);
};


#define PORT 8080

int main() {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	std::ifstream f_index("public/index.html"); //taking file as inputstream
	std::string s_index;
	if (f_index) {
		std::ostringstream ss;
		ss << f_index.rdbuf(); // reading data
		s_index = ss.str();
	}

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}
   std::string p_index = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(s_index.length()) +"\n\n" + s_index;


	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	memset(address.sin_zero, '\0', sizeof address.sin_zero);

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
		// GET REQUEST HEADERS
		parsing_request(new_socket);
//		if (test.find("GET / ") != std::string::npos || test.find("GET /index ") != std::string::npos)
//		write(new_socket, ft_toChar(p_index), ft_strlen(p_index));
//		std::cout << test << std::endl;
		printf("------------------Hello message sent-------------------");
		close(new_socket);
		close(server_fd);
		exit(1);
	}
	return 0;
}
