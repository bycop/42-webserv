//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"


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
//			std::cout << "Server[" << it->getHost() << "] - Port[" << *port_it << "]" << std::endl; // TODO: PRINT FOR TEST

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

void receiving_information(vector<int> &server_socket, Response &response, Data &data) {
	map<string, string> request_header;
	string request_body;
	int kq, new_events;
	struct kevent event_list[server_socket.size()];

	init_kqueue(server_socket, kq);
	display_banner();
	while (data.IsRunning()) {
		// CATCH ALL EVENTS
		cout << "------- Waiting for new connections -------" << endl;
		if ((new_events = kevent(kq, NULL, 0, event_list, server_socket.size(), NULL)) == -1)
			ft_error("kevent");
		for (int i = 0; i < new_events; i++) {
			int event_fd = static_cast<int>(event_list[i].ident);
			std::cout << "SOCKET FD : " << event_fd << endl;

			// CONNEXION END
			if (event_list[i].flags & EV_EOF || event_list[i].flags & EV_ERROR)
				end_connexion(data, event_fd);
			// CONNEXION ALREADY ACCEPTED
			else if (data.checkFdAlreadyAccepted(event_fd))
				process_request(event_fd, request_header, request_body, response, data);
			// ACCEPT THE SOCKET, CREATE A EVENT TO THIS SOCKET, AND ADD TO OUR SOCKET VECTOR
			else if (include_in_vector(server_socket, event_fd))
				create_connection(event_fd, kq, data);
		}
	}
	// CLOSE ALL SOCKETS
	for (vector<int>::const_iterator it = data.getSocketFdAccepted().begin(); it < data.getSocketFdAccepted().end(); ++it)
		close(*it);
	close(server_socket[0]);
}
