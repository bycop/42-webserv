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

    cout << "OPEN FILE YEAH" << endl;
	if (ifs) {
        std::ostringstream oss;
        oss << ifs.rdbuf();
        std::string file = oss.str();
		response.fillBody(file);
		response.setStatus("200 OK");
    }
	else
		response.setStatus("404 Not Found");
}


// RESPONSE MAIN
void display_page(int new_socket, std::map<std::string, std::string> request_header, bool autoindex, Response &response, string &request_body){
    string path = request_header["path"];
	DIR *dir;

	if (endsWith(path, ".py"))
		response.responseCGI(backend_page(request_header, request_body), request_header);
	else {
		if (checkError(path, response))
			create_error_page(response);
		else if (autoindex && (dir = opendir(const_cast<char *>(("." + path).c_str()))) != NULL)
			create_indexing_page(dir, path, response);
		else
			openFile("." + path, response);
			response.fillHeader(path, request_header, false);
	}
	response.response_http(new_socket);
}
