//
// Created by Alexis Lafrance on 2/8/22.
//

#include "webserv.hpp"


void process_request(int &fd, map<string, string> &request_header, string &request_body, Response &response, Data &data) {
	cout << "------- Processing the request -------" << endl << endl;
	request_header = parsing_request_header(fd, response);
	request_body = parsing_request_body(fd, request_header, response);
	display_page(fd, request_header, true, response, request_body);
	if (request_header["Connection"] == "close")
		end_connexion(data, fd);
}

void create_connection(int event_fd, int kq, Data &data) {
	struct sockaddr_in client_addr = {};
	int socket_fd, client_len = sizeof(client_addr);

	//				fcntl(socket_connection_fd, F_SETFL, O_NONBLOCK); // Todo
	if ((socket_fd = accept(event_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len)) == -1)
		ft_error("Accept socket error");
	createEvent(kq, socket_fd);
	data.pushSocketFdAccepted(socket_fd);
	cout << "✅" << " Socket connection accepted (event_fd: " << event_fd << ", socket_fd : " << socket_fd << ")" << endl;
}

void end_connexion(Data &data, int socket_fd) {
	cout << "Connexion end with socket_fd : " << socket_fd << endl;
	data.eraseSocketFd(socket_fd);
	close(socket_fd);
}