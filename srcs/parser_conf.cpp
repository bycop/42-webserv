#include "webserv.hpp"

#define INT 1
#define STRING 2
#define INT_A 3
#define STRING_A 4
#define RESTRICT_CHARS 5
#define _countof(a) (sizeof(a) / sizeof(*(a)))

bool error = false;

void display_error(string const &type, string const &key, int typeerror = 0) {
	if (typeerror)
		cerr << "\e[91m[ERROR]\e[0m Invalid type for " << type << " key at '"
			 << key << "'" << endl;
	else
		cerr << "\e[91m[ERROR]\e[0m Unrecognized " << type << " key at '"
			 << key << "'" << endl;
	error = true;
}

bool checkTypes(int type, string line, string allowed_chars = "") {
	switch (type) {
		case INT:
			for (unsigned long i = 0; i < line.length(); i++)
				if (!isdigit(line[i]) && allowed_chars.find(line[i]) == std::string::npos)
					return (false);
			break;
		case STRING:
			for (unsigned long i = 0; i < line.length(); i++)
				if (!isalnum(line[i]) && allowed_chars.find(line[i]) == std::string::npos)
					return (false);
			break;
		case INT_A:
			for (unsigned long i = 0; i < line.length(); i++)
				if (!isdigit(line[i]) && line[i] != ',' && allowed_chars.find(line[i]) == std::string::npos)
					return (false);
			break;
		case STRING_A:
			for (unsigned long i = 0; i < line.length(); i++)
				if (!isalnum(line[i]) && line[i] != ',' && allowed_chars.find(line[i]) == std::string::npos)
					return (false);
			break;
		case RESTRICT_CHARS:
			for (unsigned long i = 0; i < line.length(); i++)
				if (allowed_chars.find(line[i]) == std::string::npos)
					return (false);
			break;
	}
	return (true);
}

vector<string> split_string(string const &line) {
	vector<string> strings;
	istringstream f(line);
	string s;
	while (getline(f, s, ',')) {
		if (!s.empty())
			strings.push_back(s);
	}
	return (strings);
}

bool main_setters(string const &newdata, const int type, Data &data) {
	switch (type) {
		case 0:
			if (!checkTypes(INT, newdata))
				return (true);
			data.setWorkers(stoi(newdata));
			break;
	}
	return (false);
}

bool server_setters(string const &newdata, const int type, Server &server) {
	switch (type) {
		case 0:
			if (!checkTypes(STRING, newdata))
				return (true);
			server.setHost(newdata);
			break;
		case 1: {
			if (!checkTypes(INT_A, newdata))
				return (true);
			vector<string> strings = split_string(newdata);
			for (unsigned long i = 0; i < strings.size(); i++)
				server.getPorts().push_back(stoi(strings[i]));
			break;
		}
		case 2:
			if (!checkTypes(STRING_A, newdata, "._-*"))
				return (true);
			server.setServerName(split_string(newdata));
			break;
		case 3:
			if (!checkTypes(STRING, newdata, "./_-=*!~+"))
				return (true);
			server.setDefaultPages(newdata);
			break;
		case 4:
			if (!checkTypes(INT, newdata))
				return (true);
			server.setClientMaxBodySize(stoi(newdata));
			break;
		case 5: {
			vector<string> strings = split_string(newdata);
			if (strings.size() != 2 || !checkTypes(INT, strings[0]) || !checkTypes(STRING, strings[1], ":/"))
				return (true);
			server.setRedirectStatus(stoi(strings[0]));
			server.setRedirect(strings[1]);
			break;
		}
		case 6:
			if (!checkTypes(RESTRICT_CHARS, newdata, "onf"))
				return (true);
			if (newdata == "on")
				server.setAutoindex(true);
			else if (newdata == "off")
				server.setAutoindex(false);
			else
				return (true);
			break;
	}
	return (false);
}

bool location_setters(string const &newdata, const int type, Location &location) {
	switch (type) {
		case 0:
			if (!checkTypes(STRING_A, newdata, "."))
				return (true);
			location.setIndex(split_string(newdata));
			break;
		case 1:
			if (!checkTypes(STRING, newdata, "./_-=*!~+"))
				return (true);
			location.setRoot(newdata);
			break;
		case 2: {
			if (!checkTypes(STRING_A, newdata))
				return (true);
			location.setAllowMethods(split_string(newdata));
			for (unsigned long i = 0; i < location.getAllowMethods().size(); i++)
				if (location.getAllowMethods()[i] != "GET" && location.getAllowMethods()[i] != "POST" &&
					location.getAllowMethods()[i] != "DELETE")
					return (true);
			break;
		}
		case 3:
			if (!checkTypes(STRING, newdata, "./_-=*!~+"))
				return (true);
			location.setUploadStore(newdata);
			break;
	}
	return (false);
}

string location_path(string const &line) {

	string path;
	size_t space = 0;
	for (unsigned long i = 0; i < line.length() && isspace(line[i]); i++)
		space++;
	if (line.find("location", space) == string::npos || line.find("{", space) == string::npos)
		return (path);
	path = &line[9 + space];
	path = path.erase(path.find("{"), path.length());
	path.erase(std::remove_if(path.begin(), path.end(), ::isspace), path.end());
	return (path);
}

void location_loop(std::ifstream &file, Server &server, string const &path) {
	Location location;
	string line;
	string locationvars[4] = {"index", "root", "allow_methods", "upload_store"};
	bool find;

	location.setPath(path);
	while (std::getline(file, line)) {
		string original = line;

		find = false;
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		for (unsigned long i = 0; i < _countof(locationvars); i++)
			if (line.find(locationvars[i] + ":", 0) == 0) {
				line = &line[locationvars[i].length() + 1];
				if (line.empty())
					display_error("location", original);
				else if (location_setters(line, i, location))
					display_error("location", original, 1);
				find = true;
			}
		if (line.find("}", 0) == 0)
			break;
		else if (!find && line != "")
			display_error("location", original);
	}
	server.getLocations().push_back(location);
}

void server_loop(std::ifstream &file, Data &data) {
	Server server;
	string line;
	string servervars[7] = {"host", "port", "server_name", "default_pages",
							"client_max_body_size", "redirect", "autoindex"};
	bool find;
	while (std::getline(file, line)) {
		string original = line;

		find = false;
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		for (unsigned long i = 0; i < _countof(servervars); i++)
			if (line.find(servervars[i] + ":", 0) == 0) {
				line = &line[servervars[i].length() + 1];
				if (line.empty())
					display_error("server", original);
				else if (server_setters(line, i, server))
					display_error("server", original, 1);
				find = true;
			}
		if (line.find("}", 0) == 0) break;
		if (!find && line.find("location", 0) == 0 && !location_path(original).empty())
			location_loop(file, server, location_path(original));
		else if (!find && line != "")
			display_error("server", original);

	}
	data.getServers().push_back(server);
}

int parser_conf(Data &data, string const &file_path) {
	std::ifstream file(file_path);
	string line;
	bool find;
	string vars[1] = {"workers"};

	if (!file.is_open()) {
		cerr << "Error: file not found" << endl;
		return (1);
	}
	while (std::getline(file, line)) {
		string original = line;
		find = false;
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		for (unsigned long i = 0; i < _countof(vars) && !find; i++)
			if (line.find(vars[i] + ":", 0) == 0) {
				line = &line[vars[i].length() + 1];
				if (line.empty())
					display_error("main", original);
				else if (main_setters(line, i, data))
					display_error("main", original, 1);
				find = true;
			}
		if (!find && line.find("server{", 0) == 0)
			server_loop(file, data);
		else if (!find && line != "")
			display_error("main", original);
	}
	if (error || !data.mandatory()) {
		cerr << "Error: Exiting program" << endl;
		return (1);
	}
	return (0);
};