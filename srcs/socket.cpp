//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"
#include <fcntl.h>
#include <sys/event.h>

void create_socket(vector<int> &server_socket, sockaddr_in &address, vector<Server> &servers) {
	int enable = 1;

	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		for (vector<int>::iterator port_it = it->getPorts().begin(); port_it != it->getPorts().end(); port_it++) {
			std::cout << "Server[" << it->getHost() << "] - Port[" << *port_it << "]" << std::endl;

			int sock;
			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				perror("In socket");
				exit(EXIT_FAILURE);
			}
			server_socket.push_back(sock);
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons(*port_it);
			memset(address.sin_zero, '\0', sizeof address.sin_zero);
			if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
				perror("setsockopt(SO_REUSEADDR) failed");
			if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0) {
				perror("In bind");
				exit(EXIT_FAILURE);
			}
			fcntl(sock, F_SETFL, O_NONBLOCK);
		}
	}
}

void receiving_information(vector<int> &server_socket, sockaddr_in &address, Response &response) {
//	int new_socket;
	(void) address;
	map<string, string> request_header;
	string request_body;
	string html_content;

	int kq, new_events, socket_connection_fd, client_len;
	struct sockaddr_in client_addr;

	for (vector<int>::iterator it = server_socket.begin(); it != server_socket.end(); it++)
		if (listen(*it, 10) < 0) {
			perror("In listen");
			exit(EXIT_FAILURE);
		}

	while (true) { //TODO: CHOOSE A WAY TO STOP OUR SERVER DIFFERENTLY

	// Create a kqueue for all server sockets

		struct kevent change_event[server_socket.size()],
				event[server_socket.size()];

		client_len = sizeof(client_addr);
		kq = kqueue();
		if (kq == -1)
			perror("kqueue");

		for (vector<int>::iterator it = server_socket.begin(); it != server_socket.end(); it++) {
			EV_SET(change_event, *it, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
			if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
				perror("kevent");
				exit(1);
			}
		}
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		new_events = kevent(kq, NULL, 0, event, 1, NULL);
		if (new_events == -1) {
			perror("kevent");
			exit(1);
		}

		for (int i = 0; i < new_events; i++) {
			int event_fd = event[i].ident;
			if (event[i].flags & EV_EOF) {
				printf("Client has disconnected");
				close(event_fd);
			} else if (event_fd == server_socket[0]) {
				socket_connection_fd = accept(event_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);
				if (socket_connection_fd == -1) {
					perror("Accept socket error");
				}

				EV_SET(change_event, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0) {
					perror("kevent error");
				}
			} else if (event[i].filter & EVFILT_READ) {
				request_header = parsing_request_header(event_fd);
				request_body = parsing_request_body(event_fd, request_header);
				display_page(event_fd, request_header, true, response, request_body);
				close(event_fd);
//				printf("read %zu bytes\n", bytes_read);
			} else {
				std::cout << "oui";
			}
		}

	}
	close(server_socket[0]);
}
