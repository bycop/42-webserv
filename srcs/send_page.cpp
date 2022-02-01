//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

int 	checkError(std::string &path){
	if (path.find("//") != std::string::npos)
		return (1);
	return (0);
}

std::string ft_header(int length, std::string s, std::string content_type){
    std::string status = "HTTP/1.1 " + s;
    std::string content = "Content-Type: " + content_type;
    std::string contlength = "Content-Length: " + std::to_string(length);
    return (status + content + contlength + "\n\n");
}

std::string ft_openFile(std::string path, std::string status, std::string content_type){
    std::ifstream ifs(path);
    std::string page;
    if (ifs) {
        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string file = oss.str();
        return(ft_header(file.length(), status, content_type) + file);
    }
    return (page);
}

int display_page(int new_socket, std::map<std::string, std::string> request, bool autoindex){
    std::string file;
    string path = request["path"];
    std::string status = "200 OK\n";
	DIR *dir;
	if (checkError(path))
		file = ft_openFile("./pages/404.html", "404 Not Found\n", "text/html\n");
	else if (autoindex && (dir = opendir(const_cast<char *>(("." + path).c_str()))) != NULL)
		file = create_indexing_page(dir, path);
	else
		file = create_existing_page(path, status);
	return (write(new_socket, const_cast<char *>(file.c_str()), file.length()));
}
