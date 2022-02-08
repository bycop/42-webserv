//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"
#include <fcntl.h>
#include <sys/event.h>

void ft_error(const char *err) {
	perror(err);
	exit(EXIT_FAILURE);
}

void create_socket(vector<int> &server_socket, vector<Server> &servers) {
	int enable = 1;
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		for (vector<int>::iterator port_it = it->getPorts().begin(); port_it != it->getPorts().end(); port_it++) {
			std::cout << "Server[" << it->getHost() << "] - Port[" << *port_it << "]" << std::endl;

			int sock;
			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
				ft_error("In socket");
			server_socket.push_back(sock);
			address.sin_port = htons(*port_it);
			memset(address.sin_zero, '\0', sizeof address.sin_zero);
			if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
				ft_error("setsockopt(SO_REUSEADDR) failed");
			if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0)
				ft_error("In bind");
			if (listen(sock, 10) < 0)
				ft_error("In listen");
		}
	}
}

bool checkFd(vector<int> &server_socket, int event_fd) {
	for (unsigned long i = 0; i < server_socket.size(); i++) {
		if (event_fd == server_socket[i])
			return (true);
	}
	return (false);
}

void init_kqueue(vector<int> &server_socket, struct sockaddr_in &client_addr, int &client_len, int &kq) {
	struct kevent change_list[server_socket.size()];

	client_len = sizeof(client_addr);
	if ((kq = kqueue()) == -1)
		ft_error("kqueue");
	for (unsigned long i = 0; i < server_socket.size(); i++)
		EV_SET(&change_list[i], server_socket[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (kevent(kq, change_list, server_socket.size(), NULL, 0, NULL) == -1)
		ft_error("kevent");
}

void receiving_information(vector<int> &server_socket, Response &response, Data &data) {
	map<string, string> request_header;
	string request_body;
	int kq, new_events, socket_connection_fd, client_len;
	struct sockaddr_in client_addr = {};
	struct timespec tmout = { 5,0 }; // Todo
	struct kevent change_list[server_socket.size()], event_list[server_socket.size()];

	init_kqueue(server_socket, client_addr, client_len, kq);
	cout << endl << "------- Launching \e[93mWarningServer\e[0m -------" << endl << endl;
	while (data.IsRunning()) {
		if ((new_events = kevent(kq, NULL, 0, event_list, server_socket.size(), NULL)) == -1)
			ft_error("kevent");
		for (int i = 0; i < new_events; i++) {
			int event_fd = static_cast<int>(event_list[i].ident);
			if (event_list[i].flags & EV_EOF) {
				cout << "ALLER HOP CIAO" << endl; // Todo
				close(event_fd);
			}
			else if (checkFd(server_socket, event_fd)) {
				if ((socket_connection_fd = accept(event_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len)) == -1)
					ft_error("Accept socket error");
				cout << endl << "------- Socket connection accepted -------" << endl << endl;
				fcntl(socket_connection_fd, F_SETFL, O_NONBLOCK); // Todo
				EV_SET(change_list, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, change_list, 1, NULL, 0, &tmout) < 0)
					ft_error("kevent");
			}
			else if (event_list[i].filter & EVFILT_READ) {
				cout << endl << "------- Processing the request -------" << endl << endl;
				thread timeout();
				request_header = parsing_request_header(event_fd, response);
				request_body = parsing_request_body(event_fd, request_header, response);
				cout << "RESPONSE: " << endl;
				display_page(event_fd, request_header, true, response, request_body, data);
//				close(event_fd); // Todo
			}
		}
	}
	close(server_socket[0]);
}
