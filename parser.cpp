#include <fstream>

#include "data.hpp"
#include <cctype>

#define _countof(a) (sizeof(a) / sizeof(*(a)))

bool error = false;

void display_error(std::string type, std::string key) {
	std::cout << "\e[91m[ERROR]\e[0m Unrecognized " << type << " key at '"
			  << key << "'" << std::endl;
	error = true;
}

void setters(std::string newdata, int type, Data *data) {
	switch (type) {
		case 0:
			data->workers = newdata;
			break;
	}
}

void server_setters(std::string newdata, int type, Server *server) {
	switch (type) {
		case 0:
			server->host = newdata;
			break;
		case 1:
			server->port = newdata;
			break;
		case 2:
			server->server_name = newdata;
			break;
		case 3:
			server->default_pages = newdata;
			break;
		case 4:
			server->client_max_body_size = newdata;
			break;
		case 5:
			server->redirect = newdata;
			break;
		case 6:
			if (newdata == "on")
				server->autoindex = true;
			else
				server->autoindex = false;
			break;
	}
}

void location_setters(std::string newdata, int type, Location *location) {
	switch (type) {
		case 0:
			location->index = newdata;
			break;
		case 1:
			location->root = newdata;
			break;
		case 2:
			location->allow_methods = newdata;
			break;
	}
}

std::string location_path(std::string line) {

	std::string path;
	size_t space = 0;
	for (int i = 0; i < line.length() && isspace(line[i]); i++)
		space++;
	if (line.find("location", space) == std::string::npos || line.find("{", space) == std::string::npos)
		return (path);
	path = &line[9 + space];
	path = path.erase(path.find("{"), path.length());
	path.erase(std::remove_if(path.begin(), path.end(), ::isspace), path.end());
	return (path);
}

void location_loop(std::ifstream &file, Server *server, std::string path) {
	Location location;
	std::string line;
	std::string locationvars[3] = {"index", "root", "allow_methods"};
	bool find;

	location.path = path;
	while (std::getline(file, line)) {
		std::string original = line;

		find = false;
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		for (int i = 0; i < _countof(locationvars); i++)
			if (line.find(locationvars[i] + ":", 0) == 0) {
				line = &line[locationvars[i].length() + 1];
				location_setters(line, i, &location);
				find = true;
			}
		if (line.find("}", 0) == 0)
			break;
		else if (!find && line != "")
			display_error("location", original);
	}
	server->locations.push_back(location);
}

void server_loop(std::ifstream &file, Data *data) {
	Server server;
	std::string line;
	std::string servervars[7] = {"host", "port", "server_name", "default_pages",
								 "client_max_body_size", "redirect", "autoindex"};
	bool find;
	while (std::getline(file, line)) {
		std::string original = line;

		find = false;
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		for (int i = 0; i < _countof(servervars); i++)
			if (line.find(servervars[i] + ":", 0) == 0) {
				line = &line[servervars[i].length() + 1];
				server_setters(line, i, &server);
				find = true;
			}
		if (line.find("}", 0) == 0) break;
		if (!find && line.find("location", 0) == 0 && !location_path(original).empty())
			location_loop(file, &server, location_path(original));
		else if (!find && line != "")
			display_error("server", original);

	}

	data->servers.push_back(server);
}

int main(int ac, char **av) {
	std::string file_path;
	if (ac == 2) {
		file_path = av[1];
	} else {
		std::cout << "Usage: ./parser <file_path>" << std::endl;
		return (1);
	}

	std::ifstream file(file_path);

	if (!file.is_open()) {
		std::cout << "Error: file not found" << std::endl;
		return (1);
	}

	Data data;
	std::string line;
	bool find;

	std::string vars[1] = {"workers"};

	while (std::getline(file, line)) {
		std::string original = line;
		find = false;
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		for (int i = 0; i < _countof(vars) && !find; i++)
			if (line.find(vars[i] + ":", 0) == 0) {
				line = &line[vars[i].length() + 1];
				setters(line, i, &data);
				find = true;
			}
		if (!find && line.find("server{", 0) == 0)
			server_loop(file, &data);
		else if (!find && line != "")
			display_error("main", original);
	}
	if (error || !data.mandatory())
		std::cout << "Error: Exiting program" << std::endl;
	else
		data.print();

	return (0);
};