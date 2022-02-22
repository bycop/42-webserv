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


bool IsPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}


bool include_in_vector(vector<int> &server_socket, int event_fd) {
	for (unsigned long i = 0; i < server_socket.size(); i++) {
		if (event_fd == server_socket[i])
			return (true);
	}
	return (false);
}

string splitPartsByParts(string const& line, const char delimiter, size_t *start) {
	size_t end;
	string part;

	if (*start > line.length())
		return (part);
	end = line.find(delimiter, *start);
	if (end == string::npos) {
		part = line.substr(*start, line.length() - *start);
		*start = line.length() + 1;
		return (part);
	}
	part = line.substr(*start, end - *start);
	*start = end + 1;
	return (part);
}

bool endsWith(const string &str, const string &suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

bool startsWith(const string &str, const string &prefix)
{
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}