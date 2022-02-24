//
// Created by Alexis Lafrance on 2/8/22.
//

#include "webserv.hpp"

void createEvent(int kq, int fd) {
	struct kevent change_list = {};

	EV_SET(&change_list, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &change_list, 1, NULL, 0, NULL) < 0)
		ft_error("kevent");
}

void init_kqueue(vector<int> &server_socket, int &kq) {
	struct kevent change_list[server_socket.size()];

	// CREATE THE QUEUE
	if ((kq = kqueue()) == -1)
		ft_error("kqueue");
	// INIT STRUCTURE
	for (unsigned long i = 0; i < server_socket.size(); i++)
		EV_SET(&change_list[i], server_socket[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	// CREATE EVENT
	if (kevent(kq, change_list, static_cast<int>(server_socket.size()), NULL, 0, NULL) == -1)
		ft_error("kevent");
}