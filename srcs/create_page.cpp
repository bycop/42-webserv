//
// Created by Quentin Robert de beauchamp on 1/18/22.
//

#include "webserv.hpp"

void create_indexing_page(DIR *dir, std::string &path, Response &response, Location &location) {
	std::ostringstream mypage;
	string pathTmp = path;
	DIR *tmpdir;

	if (pathTmp[0] == '.')
		pathTmp.erase(0, 1);

	mypage << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>Index of " << pathTmp << "</title>\n"
																							"<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css\" integrity=\"sha512-9usAa10IRO0HhonpyAIVpjrylPvoDwiPUiKdWk5t3PyolY1cOd4DSE0Ga+ri4AuTroPR5aQvXU9xC6qOPnzFeg==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\" />\n"
																							"</head>\n" << std::endl;
	mypage << "<body><h1> <i class=\"fa-solid fa-triangle-exclamation\"></i> Index of " << pathTmp << "\n</h1><hr/><br>";

	struct dirent *ent;
	string path_link;
	size_t pos_root;

	ent = readdir(dir);
	while ((ent = readdir(dir)) != NULL) {
		std::string name = ent->d_name;
		path_link = pathTmp + name;
		if ((pos_root = path_link.find(location.getRoot())) != string::npos) {
			path_link.erase(pos_root, location.getRoot().length());
		}
		if ((tmpdir = opendir(const_cast<char *>((path + name).c_str()))) != NULL && name != "..")
			mypage << "<a href=\"" << path_link << "\">" << "<i class=\"fa-solid fa-folder\"></i>  " <<ent->d_name << "/" << "</a><br>";
		else if (name != "..")
			mypage << "<a href=\"" << path_link << "\">" << "<i style='font-size: 19px' class=\"fa-solid fa-file-lines\"></i>  " << ent->d_name << "</a><br>";
		else
			mypage << "<a href=\"" << path_link << "\">" << "<i style='font-size: 24px' class=\"fa-solid fa-circle-chevron-left\"></i>" << "</a><br><br>";
		if (tmpdir)
			closedir(tmpdir);
	}
	closedir (dir);
	mypage << "<br><hr/></body></html>\n";
	response.setStatus("200 OK");
	response.fillBody(mypage.str());
}

void create_error_page(Response &response, Server &server, string html_content) {
	string path = "./pages/";
	map<string, string> pages = server.getDefaultPages();
	for (map<string, string>::iterator mit = pages.begin(); mit != pages.end(); mit++){
		if (response.getStatus().find(mit->first) != string::npos) {
			if (openFile(mit->second, response))
				return;
		}
	}
	if (html_content.empty())
		html_content = response.getStatus();
	string status = response.getStatus();
	string error = (status != "200 Shutdown\n" ? "Error " : "");
	if (!openFile(path + (response.getStatus().substr(0, 3) + ".html"), response)) {
		std::ostringstream mypage;
		mypage << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>" << error << status
			   << "</title>\n</head>\n" << std::endl;
		mypage << "<body><h1>" << error << html_content << "\n</h1></body></html>";
		response.fillBody(mypage.str());
	}
}
