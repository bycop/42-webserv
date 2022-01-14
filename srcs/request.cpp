//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "global.hpp"

std::string ft_header(int length, std::string s, std::string content_type){
    std::string status = "HTTP/1.1 " + s;
    std::string content = "Content-Type: " + content_type;
    std::string contlength = "Content-Length: ";
    return (status + content + contlength + std::to_string(length) + "\n\n");
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
    return (NULL);
}

int ft_autoindex(int new_socket, std::map<std::string, std::string> request){
    std::string file;
    std::map<std::string, std::string>::iterator it = request.begin();
    std::string status = "200 OK\n";
    if (it->second == "/" || it->second == "/index")
        file = ft_openFile("./pages/index.html", status, "text/html\n");
    else if (it->second == "/test.png" || it->second == "/fuck.jpg")
        file = ft_openFile("./pages" + it->second, status, "image/png\n");
    else
        file = ft_openFile("./pages/404.html", "404 Not Found\n", "text/html\n");
    if (!file.empty()) {
        return (write(new_socket, const_cast<char *>(file.c_str()), file.length()));
    }
    return (-1);
}

