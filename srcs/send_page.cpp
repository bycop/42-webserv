//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

bool checkRights(std::string &path, Response &response) {
	struct stat results = {};

	if (stat(path.c_str(), &results)) {
		response.setStatus("404 Not Found");
		return (false);
	};
	if (!(results.st_mode & S_IRUSR) && !(results.st_mode & S_IRGRP) && !(results.st_mode & S_IROTH)) {
		response.setStatus("403 Forbidden");
		return (false);
	}
	return (true);
}

int 	checkError(std::string &path, Response &response, std::map<std::string, std::string> request_header, Location &location) {
	if (response.getStatus() != "200 OK\n" && !response.getStatus().empty())
		return (1);
	if (request_header["version"].empty() || request_header["version"] != "HTTP/1.1")
		response.setStatus("505 HTTP Version Not Supported");
	else if (request_header["method"].empty() ||
			 (request_header["method"] != "GET" && request_header["method"] != "POST" &&
			  request_header["method"] != "DELETE"))
		response.setStatus("501 Not Implemented");
	else if (!contains(location.getAllowMethods(), request_header["method"]))
		response.setStatus("405 Method Not Allowed");
	else if (!checkRights(path, response) || (response.getStatus() != "200 OK\n" && !response.getStatus().empty()))
		return (1);

	std::ifstream ifs(path);
	if (response.getStatus() != "200 OK\n" && !response.getStatus().empty())
		return (1);
	if (!ifs || path.find("//") != std::string::npos)
		response.setStatus("404 Not Found");
	else if (response.getStatus() == "200 OK\n")
		return (0);
	return (1);
}
bool openFile(std::string path, Response &response){

	if (!checkRights(path, response))
		return (false);

	std::ifstream ifs(path);
    std::string page;
	if (ifs) {
        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string file = oss.str();
		response.fillBody(file);
    }
	else {
		response.setStatus("404 Not Found");
		return (false);
	}
	return (true);
}

void add_slash_to_directory(string &path) {
	DIR *dir;

	if ((dir = opendir(const_cast<char *>((path).c_str()))) != NULL && path[path.length() - 1] != '/')
		path.insert(path.length(), "/");
	if (dir)
		closedir(dir);
}

void deleteFile(string &pathModify, Response &response, map<string, string> &request_header, Server &server) {
	DIR *dir = NULL;

	if (checkRights(pathModify, response) && (dir = opendir(const_cast<char *>(pathModify.c_str()))) == NULL) {
		std::remove(pathModify.c_str());
		response.setStatus("200 OK");
		response.fillBody("<html><body><h1>File deleted.</h1></body></html>");
		response.fillHeader(pathModify, request_header, true);
	}
	else {
		create_error_page(response, server);
		if (dir)
			closedir(dir);
	}
}

void sendAutoIndex(string &pathModify, Response &response, Server &server, Location &location, DIR *dir) {
	struct dirent *ent;
	string path;
	DIR *tmpdir;

	while ((ent = readdir(dir)) != NULL) {
		tmpdir = NULL;
		string name = ent->d_name;
		path = pathModify + (endsWith(pathModify, "/") ? "" : "/") + name;

		if (contains(location.getIndex(), name) && (tmpdir = opendir(const_cast<char *>(path.c_str()))) == NULL) {
			if (!openFile(path, response))
				create_error_page(response, server);
			break;
		}
		if (tmpdir)
			closedir(tmpdir);
	}

	if (ent == NULL) {
		response.setStatus("404 Not Found");
		create_error_page(response, server);
	}
	closedir(dir);
}

void display_page(int &new_socket, std::map<std::string, std::string> &request_header, Response &response, string &request_body, Server &server, Location &location) {
	DIR *dir;
	string pathModify = (!location.getRoot().empty() ? location.getRoot() : ".") + request_header["path"];

	add_slash_to_directory(pathModify);
	if (checkError(pathModify, response, request_header, location)) {
		create_error_page(response, server);
		response.fillHeader(pathModify, request_header, false);
	}
	else if (request_header["method"] == "DELETE")
		deleteFile(pathModify, response, request_header, server);
	else if (endsWith(pathModify, ".py") || (endsWith(pathModify, ".php")))
		response.responseCGI(backend_page(request_header, request_body, location, server), request_header, server);
	else {
		if ((dir = opendir(const_cast<char *>(pathModify.c_str()))) != NULL)
			server.isAutoindex() ? create_indexing_page(dir, pathModify, response, location) : sendAutoIndex(pathModify, response, server, location, dir);
		else if (!openFile(pathModify, response))
				create_error_page(response, server);
		response.fillHeader(pathModify, request_header, false);
	}
	response.response_http(new_socket);
	response.resetResponse();
}
