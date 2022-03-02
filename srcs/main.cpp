//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"


int check_command_exist(string const &command) {
	string all_command = "command -v " + command + " > /dev/null";
	int ret = system(all_command.c_str());
	if (WEXITSTATUS(ret) == 1) {
		cerr << "You need to install " << command << " to run warning server" << endl;
		return (1);
	}
	return (0);
}

int main(int ac, char **av) {
	Response response;
	vector<int> server_socket;
	string file_path = (ac == 2) ? av[1] : "webserv.conf";
	Data data;

	if (check_command_exist("python") || check_command_exist("php-cgi"))
		return (1);
	if (parser_conf(data, file_path))
		return (1);
    create_socket(server_socket, data.getServers());
	receiving_information(server_socket, response, data);
	return 0;
}
