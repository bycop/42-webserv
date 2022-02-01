//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

int main(int ac, char **av) {
	int server_socket;
	struct sockaddr_in address;
	string file_path = (ac == 2) ? av[1] : "webserv.conf";
	Data data;

	if (parser_conf(data, file_path))
		return (1);
	data.print();
    create_socket(server_socket, address);
	receiving_information(server_socket, address);
	return 0;
}
