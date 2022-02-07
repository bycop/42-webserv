//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

int 	checkError(std::string &path, Response &response){
	if (path.find("//") != std::string::npos) {
		response.setStatus("404 Not Found");
		return (1);
	}
	return (0);
}

void openFile(std::string path, Response &response){
    std::ifstream ifs(path);
    std::string page;
    if (ifs) {
        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string file = oss.str();
		response.setStatus("200 OK");
        response.fillHeader(file, path);
    }
	else
		response.setStatus("404 Not Found");
}

void display_page(int new_socket, std::map<std::string, std::string> request_header, bool autoindex, Response &response, string &request_body){
    string path = request_header["path"];
	cout << path << endl;
	DIR *dir;
	if (checkError(path, response))
		create_error_page(path, response);
	else if (path.find(".py") != string::npos)
		response.fillHeaderCGI(backend_page(request_header, request_body));
	else if (autoindex && (dir = opendir(const_cast<char *>(("." + path).c_str()))) != NULL)
		create_indexing_page(dir, path, response);
	else
		openFile("." + path, response);
	write(new_socket, const_cast<char *>(response.getResponse().c_str()), response.getResponse().length());
}
