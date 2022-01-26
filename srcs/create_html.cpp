//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#include "webserv.hpp"

std::string create_indexing_page(DIR *dir, std::string path){
	std::ostringstream mypage;
	std::string dir_p = ".";
	std::string status = "200 OK\n";
	std::string content_type = "text/html\n";
	mypage << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>Index of " << path << "</title>\n</head>\n" << std::endl;
	mypage << "<body><h1>Index of " << path << "\n</h1><hr/><br>";
	struct dirent *ent;
	ent = readdir(dir);

	if (path[path.length() - 1] != '/')
		path.insert(path.length(), "/");
	while ((ent = readdir(dir)) != NULL) {
		std::string name = ent->d_name;
		if (opendir(const_cast<char *>(("." + path + name).c_str())) != NULL && name != "..") {
			mypage << "<a href=\"" << path << name << "\">" << ent->d_name << "/" << "</a><br>";
		}
		else
			mypage << "<a href=\"" << path << name << "\">" << ent->d_name << "</a><br>";
	}
	closedir (dir);
	mypage << "<br><hr/></body></html>\n";
	return (ft_header(mypage.str().length(), status, content_type) + mypage.str());
}

std::string		create_existing_page(std::string &path, std::string &status) {
	std::string file;
	if (path == "/" || path == "/pages/index.html")
		file = ft_openFile("./pages/index.html", status, "text/html\n");
	else if (path == "/pages/form.html")
		file = ft_openFile("./pages/form.html", status, "text/html\n");
	else if (path == "/pages/file.html")
		file = ft_openFile("./pages/file.html", status, "text/html\n");
	else if (path == "/pages/test.png" || path == "/pages/fuck.jpg")
		file = ft_openFile("." + path, status, "image/png\n");
	else
		file = ft_openFile("./pages/404.html", "404 Not Found\n", "text/html\n");
	return (file);
}
