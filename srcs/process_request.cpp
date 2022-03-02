//
// Created by Alexis Lafrance on 2/8/22.
//

#include "webserv.hpp"


Server findServerForHost(string &header_host, Data &data, Response &response) {

	if (header_host.find(':') != string::npos) { // IF WE HAVE HOST:PORT
		int port;
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
	response.setStatus("400 Bad Request");
	return (Server());
}

Location findLocationForServer(string &header_path, Server &server, Response &response) {
	string file[3]; // [0] = directory; [1] = filename; [2] = extension
	Location location;
	unsigned long pos;

	if ((pos = header_path.rfind('/')) != string::npos) {
		file[0] = header_path.substr(0, pos + 1);
		file[1] = header_path.substr(pos + 1);
		if ((pos = file[1].rfind('.')) != string::npos)
			file[2] = file[1].substr(pos);
	}
	else {
		file[1] = header_path;
		file[2] = (pos = file[1].rfind('.')) != string::npos ? file[1].substr(pos) : "";
	}
	for (vector<Location>::iterator it = server.getLocations().begin(); it != server.getLocations().end(); it++) {
		if (it->getPath() == "/" || file[0] == it->getPath() || file[0] + "/" == it->getPath()) {
			location = *it;
			if (it->getPath() != "/")
				break;
		}
	}
	for (vector<Location>::iterator it = server.getLocations().begin(); it != server.getLocations().end(); it++) {
		if (file[2] == it->getPath()) {
			if (!it->getAllowMethods().empty())
				location.setAllowMethods(it->getAllowMethods());
			if (it->getUploadStore().length() > 0)
				location.setUploadStore(it->getUploadStore());
			if (it->getRoot().length() > 0)
				location.setRoot(it->getRoot());
			break;
		}
	}
	if (location.isEmpty())
		response.setStatus("418 I'm a teapot");
	else {
		if (location.getUploadStore().empty())
			location.setUploadStore(DEFAULT_UPLOAD_FOLDER);
		if (location.getAllowMethods().empty()) {
			location.getAllowMethods().push_back("GET");
			location.getAllowMethods().push_back("POST");
		}
	}
	return (location);
}

bool checkForStop(Data &data, const string &r_request_body, const map<string, string> &request_header, Response &response) {
	string token = "token=" + string(STOP_PASSWORD);
	if (request_header.find("path")->second == "/stop" && r_request_body == token) {
		data.setIsRunning(false);
		response.setStatus("200 Shutdown");
	}
	return (false);
}

void process_request(int &fd, Response &response, Data &data) {
	cout << "------- Processing the request -------" << endl << endl;
	map<string, string> request_header;
	string r_request_body;
	Server server;
	Location location;

	request_header = parsing_request_header(response, readHeader(fd));
	server = findServerForHost(request_header["Host"], data, response);
	location = findLocationForServer(request_header["path"], server, response);
	process_body(fd, r_request_body, response, server, request_header);
	checkForStop(data, r_request_body, request_header, response);
	bool stat = response.getStatus() == "504 Gateway Timeout\n";
	display_page(fd, request_header, response, r_request_body, server, location);
	if (request_header["Connection"] == "close" || stat)
		end_connexion(data, fd);
}

bool 	check_error_body(Server &server, Response &response, map<string, string> & request_header ) {
	long long length;
	stringstream ss(request_header["Content-Length"]);
	ss >> length;

	if (request_header.find("Content-Length") == request_header.end())
		response.setStatus("411 Length Required");
	else if (length > 22548578304 || length < 0 || request_header.find("Content-Length")->second.find_first_not_of("0123456789") != string::npos)
		response.setStatus("400 Bad Request");
	else if (atoi(request_header["Content-Length"].c_str()) > server.getClientMaxBodySize() * pow(10, 6))
		response.setStatus("504 Gateway Timeout");
	else
		return (true);
	return (false);
}

void process_body(int fd, string &read_request_body, Response &response, Server server, map<string, string> &request_header) {
	if (request_header["method"] != "POST")
		return ;
	if (!check_error_body(server, response, request_header))
		return ;
	read_request_body = readBody(fd, request_header);
	if (request_header.find("Transfer-Encoding") != request_header.end() &&
			request_header.find("Transfer-Encoding")->second == "chunked")
		read_request_body = defragment_request_body(read_request_body);
}

void create_connection(int event_fd, int kq, Data &data) {
	struct sockaddr_in client_addr = {};
	int socket_fd, client_len = sizeof(client_addr);

	if ((socket_fd = accept(event_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len)) == -1)
		ft_error("Accept socket error");
	createEvent(kq, socket_fd);
	data.pushSocketFdAccepted(socket_fd);
	cout << "✅" << " Socket connection accepted (event_fd: " << event_fd << ", _socket_fd : " << socket_fd << ")"
		 << endl;
}

void end_connexion(Data &data, int socket_fd) {
	cout << "Connexion end with _socket_fd : " << socket_fd << endl;
	data.eraseSocketFd(socket_fd);
	close(socket_fd);
}