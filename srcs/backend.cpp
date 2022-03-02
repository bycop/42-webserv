//
// Created by Alexis Lafrance on 24/01/2022.
//

#include "webserv.hpp"

void setenv_cgi(map<string, string> & request_header, Server &server) {
	char tmp[2048];
	getcwd(tmp, 2048);

	if (!endsWith(tmp, "/"))
		strcat(tmp, "/");
	(void)server;
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SERVER_NAME", server.getHost().c_str(), 1);
	setenv("SERVER_SOFTWARE", "WarningServer", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("SERVER_PORT", ITOA(server.getPort()).c_str(), 1);
	setenv("REQUEST_METHOD",request_header["method"].c_str(), 1);
	setenv("REQUEST_URI",tmp, 1);
	setenv("PATH_INFO", tmp,  1);
	string script_filename(tmp);
	script_filename += request_header["path"].substr(1, request_header["path"].length() - 1);
	setenv("SCRIPT_FILENAME", script_filename.c_str(),  1);
	setenv("SCRIPT_NAME", request_header["path"].c_str(), 1);
	setenv("REDIRECT_STATUS", "CGI", 1);
	setenv("DOCUMENT_ROOT", tmp, 1);
	setenv("QUERY_STRING", request_header["query"].c_str(), 1);
	setenv("CONTENT_TYPE",request_header["Content-Type"].c_str(), 1);
	setenv("CONTENT_LENGTH",request_header["Content-Length"].c_str(), 1);
	setenv("HTTP_ACCEPT",request_header["Accept"].c_str(), 1);
	setenv("HTTP_USER_AGENT",request_header["User-Agent"].c_str(), 1);
	setenv("HTTP_REFERER",request_header["Referer"].c_str(), 1);
	setenv("HTTP_COOKIE",request_header["Cookie"].c_str(), 1);
}

void launch_backend_file(map<string, string> &request_header, Location &location) {
	std::string filename = request_header["path"];
	string command;
	const string  &directory = location.getUploadStore();
	std::string launch_cgi;

	filename.erase(0, 1);
	// CHOOSE COMMAND INTERPRETER
	if (endsWith(request_header["path"], ".py"))
		launch_cgi = "python3";
	else
		launch_cgi = "php-cgi";
	// ADD DIRECTORY
	launch_cgi += " " + string(getenv("DOCUMENT_ROOT")) + filename;
	// CREATE DIRECTORY IF NOT EXIST
	DIR* dir = opendir(directory.c_str());
	if (!dir) {
		command = "mkdir " + directory;
		system(command.c_str());
	}
	// CALL COMMAND
	command = "cd " + directory + " && " + launch_cgi + ">" + getenv("DOCUMENT_ROOT") +  "bin/tmp";
	system(command.c_str());
	if (dir)
		closedir(dir);
}

string send_body(map<string, string> &request_header, string &request_body, Location &location) {
	pid_t pid;
	int p[2];
	string html_content;

	pipe(p);
	pid = fork();
	if (pid == -1) {
		perror("fork");
		return (html_content);
	}
	else if (pid == 0) { // CHILD
		dup2(p[0], 0);
		launch_backend_file(request_header, location);
		close(p[0]);
		exit(0);
	}
	else { // PARENT
		write(p[1], request_body.c_str(), request_body.length());
		close(p[1]);
		close(p[0]);
		waitpid(pid, NULL, 0);
		char buffer[1];
		bzero(buffer, 2);
		string filename(getenv("DOCUMENT_ROOT"));
		filename += "/bin/tmp";
		html_content = readFile(filename);
		remove(filename.c_str());
	}
	return (html_content);
}
// MISE EN PLACE CGI
string backend_page(map<string, string> &request_header, string &request_body, Location &location, Server &server) {
	setenv_cgi(request_header, server);
	return (send_body(request_header, request_body, location));
}
