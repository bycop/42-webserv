//
// Created by Alexis Lafrance on 2/8/22.
//

#include "webserv.hpp"

void display_banner() {
	cout << "██     ██  █████  ██████  ███    ██ ██ ███    ██  ██████              ███████ ███████ ██████  ██    ██ ███████ ██████  \n" <<
		 	"██     ██ ██   ██ ██   ██ ████   ██ ██ ████   ██ ██                   ██      ██      ██   ██ ██    ██ ██      ██   ██ \n" <<
		 	"██  █  ██ ███████ ██████  ██ ██  ██ ██ ██ ██  ██ ██   ███             ███████ █████   ██████  ██    ██ █████   ██████  \n" <<
		 	"██ ███ ██ ██   ██ ██   ██ ██  ██ ██ ██ ██  ██ ██ ██    ██                  ██ ██      ██   ██  ██  ██  ██      ██   ██ \n" <<
			 " ███ ███  ██   ██ ██   ██ ██   ████ ██ ██   ████  ██████              ███████ ███████ ██   ██   ████   ███████ ██   ██ \n";
}

bool include_in_vector(vector<int> &server_socket, int event_fd) {
	for (unsigned long i = 0; i < server_socket.size(); i++) {
		if (event_fd == server_socket[i])
			return (true);
	}
	return (false);
}

bool endsWith(const string &str, const string &suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

bool startsWith(const string &str, const string &prefix)
{
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}