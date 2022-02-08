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

bool already_open(vector<int> &ports, int port) {
	for (vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
		if (*it == port)
			return (true);
	return (false);
}

void create_socket(vector<int> &server_socket, vector<Server> &servers) {
	int enable = 1;
	struct sockaddr_in address;
	vector<int> ports;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		std::cout << "Server[" << it->getHost() << "] - Port[" << it->getPort() << "]" << std::endl;

		if (already_open(ports, it->getPort()))
			continue;

		int sock;
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
			ft_error("In socket");
		server_socket.push_back(sock);
		address.sin_port = htons(it->getPort());
		memset(address.sin_zero, '\0', sizeof address.sin_zero);
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			ft_error("setsockopt(SO_REUSEADDR) failed");
		if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0)
			ft_error("In bind");
		if (listen(sock, 10) < 0)
			ft_error("In listen");
		ports.push_back(it->getPort());
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


Server findServerForHost(string header_host, Data &data, Response &response) {
	int port;


	std::cout << "Header: " << header_host << endl;
	if (header_host.find(':') != string::npos) { // IF WE HAVE HOST:PORT
		size_t start = 0;
		string host = splitPartsByParts(header_host, ':', &start);
		port = atoi(splitPartsByParts(header_host, ':', &start).c_str());
		for (vector<Server>::iterator it = data.getServers().begin(); it != data.getServers().end(); it++) {
			if (it->getPort() == port && it->getHost() == host)
				return (*it);
		}
	}
	else if (header_host == "localhost" || header_host == "127.0.0.1") { // Special cases for port 80
		for (vector<Server>::iterator it = data.getServers().begin(); it != data.getServers().end(); it++) {
			if (it->getPort() == 80 && it->getHost() == header_host)
				return (*it);
		}
	}
	else { // IF WE HAVE SERVER NAME
		for (vector<Server>::iterator it = data.getServers().begin(); it != data.getServers().end(); it++) {
			for (vector<string>::iterator it2 = it->getServerName().begin(); it2 != it->getServerName().end(); it2++) {
				if (*it2 == header_host)
					return (*it);
			}
		}
	}
	// IF WE HAVE A ERROR
	response.setStatus("418 I'm a teapot");
	return (Server());
}

void receiving_information(vector<int> &server_socket, Response &response, Data &data) {
	map<string, string> request_header;
	string request_body;
	int kq, new_events, socket_connection_fd, client_len;
	struct sockaddr_in client_addr = {};
	struct timespec tmout = {5, 0}; // Todo
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
				if ((socket_connection_fd = accept(event_fd, (struct sockaddr *) &client_addr,
												   (socklen_t *) &client_len)) == -1)
					ft_error("Accept socket error");
				cout << endl << "------- Socket connection accepted -------" << endl << endl;
				fcntl(socket_connection_fd, F_SETFL, O_NONBLOCK); // Todo
				EV_SET(change_list, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, change_list, 1, NULL, 0, &tmout) < 0)
					ft_error("kevent");
			}
			else if (event_list[i].filter & EVFILT_READ) {
				cout << endl << "------- Processing the request -------" << endl << endl;
				request_header = parsing_request_header(event_fd, response);
				request_body = parsing_request_body(event_fd, request_header, response);
				Server server = findServerForHost(request_header["Host"], data, response);
				display_page(event_fd, request_header, true, response, request_body, server);
				close(event_fd); // Todo
			}
		}
	}
	close(server_socket[0]);
}
