//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "socket.hpp"

std::string ft_header(int length){
	std::string status = "HTTP/1.1 200 OK\n";
	std::string content = "Content-Type: application/json\n";
	std::string length = "Content-Length: ";
	return (status + content + length + std::to_string(length) + "\n\n");
}

std::string ft_openFile(std::string path){
	std::ifstream ifs(path);
	std::string page;
	if (ifs) {
		std::ostringstream oss;
		oss << ifs.rdbuf();
		std::string file = oss.str();
		return(ft_header(file.length()) + file);
	}
	return (NULL);
}

int ft_createSocket() {
	int server_fd, new_socket;
	long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
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
			ft_autoindex(new_socket, ft_openFile("./page/index.html"));
		else if (test.find("GET /test.png ") != std::string::npos)
			ft_autoindex(new_socket, ft_openFile("./pages/test.png"));
		else
			ft_autoindex(new_socket, ft_openFile("./pages/404.html "));
		std::cout << test << std::endl;
		close(new_socket);
	}
}


//std::ifstream f_json("./pages/test.json"); //taking file as inputstream
//std::string s_json;
//if (f_json) {
//std::ostringstream ss;
//ss << f_json.rdbuf(); // reading data
//s_json = ss.str();
//}
//std::ifstream f_index("./pages/index.html"); //taking file as inputstream
//std::string s_index;
//if (f_index) {
//std::ostringstream ss;
//ss << f_index.rdbuf(); // reading data
//s_index = ss.str();
//}
//
//std::ifstream f_jpeg("./pages/test.png"); //taking file as inputstream
//std::string s_jpeg;
//if (f_jpeg) {
//std::ostringstream ss;
//ss << f_jpeg.rdbuf(); // reading data
//s_jpeg = ss.str();
//}

//std::string p_json = "HTTP/1.1 200 OK\nContent-Type: application/json\nContent-Length: " + std::to_string(s_json.length()) + "\n\n" + s_json;
//std::string p_404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(s_404.length()) +"\n\n" + s_404;
//std::string p_index = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(s_index.length()) +"\n\n" + s_index;
//std::string p_jpeg = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: " + std::to_string(s_jpeg.length()) +"\n\n" + s_jpeg;

// Creating socket file descriptor
