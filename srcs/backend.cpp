//
// Created by Alexis Lafrance on 24/01/2022.
//

#include "webserv.hpp"
void setenv_cgi(map<string, string> & request_header) {
	setenv("CONTENT_TYPE",request_header["Content-Type"].c_str(), 1);
	setenv("REQUEST_METHOD",request_header["method"].c_str(), 1);
	setenv("CONTENT_LENGTH",request_header["Content-Length"].c_str(), 1);
}


void launch_backend_file(map<string, string> &request_header) {
	std::string filename = request_header["path"];
	filename.erase(0, 1);
	std::string command = "python3";
	command += " " + filename;
	system(command.c_str());
}

string send_body(map<string, string> &request_header, string &request_body) {
	pid_t pid;
	int p[2];
	string html_content;

	pipe(p);
	pid = fork();
	if (pid == -1) {
		perror("fork");
		return (html_content);
	}
	else if (pid == 0) {// CHILD
		dup2(p[0], 0);
		dup2(p[1], 1);
		launch_backend_file(request_header);
		write(1, "\0", 1);
		close(p[1]);
		close(p[0]);
		exit(0);
	}
	else { // PARENT
		write(p[1], request_body.c_str(), request_body.length());
		close(p[1]);
		waitpid(pid, NULL, 0);
		char buffer[1];
		bzero(buffer, 2);
		while (read(p[0], buffer, 1) && buffer[0] != '\0')
			html_content += buffer[0];
		close(p[0]);
	}
	return (html_content);
}
// MISE EN PLACE CGI
string backend_page(map<string, string> &request_header, string &request_body) {
	setenv_cgi(request_header);
	return (send_body(request_header, request_body));
}
