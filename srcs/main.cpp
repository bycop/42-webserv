//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "webserv.hpp"

int main() {
    Response response;
	int server_socket;
	struct sockaddr_in address;

    create_socket(server_socket, address);
	receiving_information(server_socket, address, response);
	return 0;
}
