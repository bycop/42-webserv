//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#include "webserv.hpp"

void create_indexing_page(DIR *dir, std::string &path, Response &response){
	std::ostringstream mypage;
	string pathTmp = path;

c	if (pathTmp[0] == '.')
		pathTmp.erase(0, 1);
	mypage << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>Index of " << pathTmp << "</title>\n</head>\n" << std::endl;
	mypage << "<body><h1>Index of " << pathTmp << "\n</h1><hr/><br>";
	struct dirent *ent;
	//Ne pas afficher le "."
	ent = readdir(dir);

	while ((ent = readdir(dir)) != NULL) {
		std::string name = ent->d_name;
		if (opendir(const_cast<char *>((path + name).c_str())) != NULL && name != "..")
			mypage << "<a href=\"" << pathTmp << name << "\">" << ent->d_name << "/" << "</a><br>";
		else
			mypage << "<a href=\"" << pathTmp << name << "\">" << ent->d_name << "</a><br>";
	}
	closedir (dir);
	mypage << "<br><hr/></body></html>\n";
	response.setStatus("200 OK");
	response.fillBody(mypage.str());
}

void create_error_page(Response &response, Server &server) {
	string path = "./pages/";
	map<string, string> pages = server.getDefaultPages();
	for (map<string, string>::iterator mit = pages.begin(); mit != pages.end(); mit++){
		if (response.getStatus().find(mit->first) != string::npos) {
			openFile(mit->second, response);
			return;
		}
	}
	string status = response.getStatus();
	if (!openFile(path + (response.getStatus().substr(0, 3) + ".html"), response)) {
		std::ostringstream mypage;
		mypage << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>Error " << status
			   << "</title>\n</head>\n" << std::endl;
		mypage << "<body><h1>Error " << status << "\n</h1></body></html>";
		response.fillBody(mypage.str());
	}
}
