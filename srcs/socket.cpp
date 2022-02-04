//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"
#include <fcntl.h>
#include <sys/event.h>

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
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
}

void receiving_information(int &server_socket, sockaddr_in &address, Response &response) {
//	int new_socket;
	(void) address;
	map<string, string> request_header;
	string request_body;
	string html_content;

	int kq, new_events, socket_connection_fd, client_len;
	struct kevent change_event[4],
			event[4];
	struct sockaddr_in client_addr;
	if (listen(server_socket, 10) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	client_len = sizeof(client_addr);

	kq = kqueue();
	if (kq == -1)
		perror("kqueue");
	EV_SET(change_event, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		exit(1);
	}
	while (true) { //TODO: CHOOSE A WAY TO STOP OUR SERVER DIFFERENTLY

		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		new_events = kevent(kq, NULL, 0, event, 1, NULL);
		if (new_events == -1) {
			perror("kevent");
			exit(1);
		}

		for (int i = 0; new_events > i; i++) {
			int event_fd = event[i].ident;

			if (event[i].flags & EV_EOF) {
				printf("Client has disconnected");
				close(event_fd);
			} else if (event_fd == server_socket) {
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
			}
		}

	}
	close(server_socket);
}
