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

void ft_openFile(std::string path, std::string content_type, Response &response){
    std::ifstream ifs(path);
    std::string page;
    if (ifs) {
        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string file = oss.str();
        response.fillHeader(file, response.getStatus(), content_type);
		response.setBody(file);
    }
}

void display_page(int new_socket, std::map<std::string, std::string> request, bool autoindex, Response &response){
    string path = request["path"];
//    std::string status = "200 OK ";
	DIR *dir;
	if (checkError(path, response))
		ft_openFile("./pages/404.html", "text/html", response);
	else if (autoindex && (dir = opendir(const_cast<char *>(("." + path).c_str()))) != NULL)
		create_indexing_page(dir, path, response);
	else
		create_existing_page(path, response);
	write(new_socket, const_cast<char *>(response.getResponse().c_str()), response.getLength());
}
