//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#include "global.hpp"

std::string create_page(DIR *dir, std::string path){
	std::ostringstream mypage;
	std::string status = "200 OK\n";
	std::string content_type = "text/html\n";
	mypage << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>Index of " << path << "</title>\n</head>\n" << std::endl;
	mypage << "<body><h1>Index of " << path << "\n</h1><hr/><br>";
	struct dirent *ent;
	ent = readdir(dir);
	while ((ent = readdir(dir)) != NULL) {
//		if (opendir(const_cast<char *>("." + (ent->d_name))) != NULL)
//			mypage << "<a href=\"" << ent->d_name << "\">" << ent->d_name << "</a><br>";
//		else
			mypage << "<a href=\"" << ent->d_name << "\">" << ent->d_name << "</a><br>";
	}
	closedir (dir);
	mypage << "<br><hr/></body></html>\n";
	return (ft_header(mypage.str().length(), status, content_type) + mypage.str());
}
