//
// Created by Alexis Lafrance on 24/01/2022.
//

#include "webserv.hpp"



void print_env() { // TODO: JUST FOR TEST
	cout << "ENV : " << endl;
	cout << "GATEWAY_INTERFACE : " << getenv("GATEWAY_INTERFACE") << endl;
	cout << "SERVER_NAME : " << getenv("SERVER_NAME") << endl;
	cout << "SERVER_SOFTWARE : " << getenv("SERVER_SOFTWARE") << endl;
	cout << "SERVER_PROTOCOL : " << getenv("SERVER_PROTOCOL") << endl;
	cout << "SERVER_PORT : " << getenv("SERVER_PORT") << endl;
	cout << "REQUEST_METHOD : " << getenv("REQUEST_METHOD") << endl;
	cout << "PATH_INFO : " << getenv("PATH_INFO") << endl;
	cout << "PATH_TRANSLATED : " << getenv("PATH_TRANSLATED") << endl;
	cout << "SCRIPT_NAME : " << getenv("SCRIPT_NAME") << endl;
	cout << "DOCUMENT_ROOT : " << getenv("DOCUMENT_ROOT") << endl;
	cout << "QUERY_STRING : " << getenv("QUERY_STRING") << endl;
	cout << "CONTENT_TYPE : " << getenv("CONTENT_TYPE") << endl;
	cout << "CONTENT_LENGTH : " << getenv("CONTENT_LENGTH") << endl;
	cout << "HTTP_ACCEPT : " << getenv("HTTP_ACCEPT") << endl;
	cout << "HTTP_USER_AGENT : " << getenv("HTTP_USER_AGENT") << endl;
	cout << "HTTP_REFERER : " << getenv("HTTP_REFERER") << endl;
}

void setenv_cgi(map<string, string> & request_header) {
	char tmp[256];
	getcwd(tmp, 256);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SERVER_NAME", "localhost", 1); // TODO: CHANGE WITH CONFIGURATION FILE
	setenv("SERVER_SOFTWARE", "WarningServer", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("SERVER_PORT", "8080", 1); // TODO: CHANGE WITH CONFIGURATION FILE
	setenv("REQUEST_METHOD",request_header["method"].c_str(), 1);
	setenv("PATH_INFO", request_header["path_info"].c_str(), 1);
	setenv("PATH_TRANSLATED", request_header["path_translated"].c_str(), 1);
	setenv("SCRIPT_NAME", request_header["path"].c_str(), 1);
	setenv("DOCUMENT_ROOT", tmp, 1);
	setenv("QUERY_STRING", request_header["query"].c_str(), 1);
	setenv("CONTENT_TYPE",request_header["Content-Type"].c_str(), 1);
	setenv("CONTENT_LENGTH",request_header["Content-Length"].c_str(), 1);
	setenv("HTTP_ACCEPT",request_header["Accept"].c_str(), 1);
	setenv("HTTP_USER_AGENT",request_header["User-Agent"].c_str(), 1);
	setenv("HTTP_REFERER",request_header["Referer"].c_str(), 1);
//	print_env();
}

void launch_backend_file(map<string, string> &request_header) {
	std::string filename = request_header["path"];
	filename.erase(0, 1);
	std::string command = "python";
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
		(void)request_body;
		close(p[0]);
	}
	return (html_content);
}
// MISE EN PLACE CGI
string backend_page(map<string, string> &request_header, string &request_body) {
	setenv_cgi(request_header);
	return (send_body(request_header, request_body));
}
