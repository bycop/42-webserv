//
// Created by Sameo Fournion on 1/12/22.
//

// Server side C/C++ program to demonstrate Socket programming

#include<iostream>
#include<fstream>
#include<sstream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

size_t ft_strlen(std::string str) {
	return (str.length());
}

char *ft_toChar(std::string str) {
	char *c = const_cast<char *>(str.c_str());
	return (c);
};


#define PORT 8080

int main(int argc, char const *argv[]) {
	int server_fd, new_socket;
	long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	std::ifstream f_404("404.html"); //taking file as inputstream
	std::string s_404;
	if (f_404) {
		std::ostringstream ss;
		ss << f_404.rdbuf(); // reading data
		s_404 = ss.str();
	}
	std::ifstream f_json("test.json"); //taking file as inputstream
	std::string s_json;
	if (f_json) {
		std::ostringstream ss;
		ss << f_json.rdbuf(); // reading data
		s_json = ss.str();
	}
	std::ifstream f_index("index.html"); //taking file as inputstream
	std::string s_index;
	if (f_index) {
		std::ostringstream ss;
		ss << f_index.rdbuf(); // reading data
		s_index = ss.str();
	}

	std::ifstream f_jpeg("test.png"); //taking file as inputstream
	std::string s_jpeg;
	if (f_jpeg) {
		std::ostringstream ss;
		ss << f_jpeg.rdbuf(); // reading data
		s_jpeg = ss.str();
	}

	std::string p_json = "HTTP/1.1 200 OK\nContent-Type: application/json\nContent-Length: " + std::to_string(s_json.length()) + "\n\n" + s_json;
	std::string p_404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(s_404.length()) +"\n\n" + s_404;
	std::string p_index = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(s_index.length()) +"\n\n" + s_index;
	std::string p_jpeg = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: " + std::to_string(s_jpeg.length()) +"\n\n" + s_jpeg;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}


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

		char buffer[30000] = {0};
		valread = read(new_socket, buffer, 30000);
		std::string test = buffer;
		if (test.find("GET / ") != std::string::npos || test.find("GET /index ") != std::string::npos)
			write(new_socket, ft_toChar(p_index), ft_strlen(p_index));
		else if (test.find("GET /json ") != std::string::npos)
			write(new_socket, ft_toChar(p_json), ft_strlen(p_json));
		else if (test.find("GET /test.png ") != std::string::npos)
			write(new_socket, ft_toChar(p_jpeg), ft_strlen(p_jpeg));
		else
			write(new_socket, ft_toChar(p_404), ft_strlen(p_404));

		std::cout << test << std::endl;
		printf("------------------Hello message sent-------------------");
		close(new_socket);
	}
	return 0;
}
