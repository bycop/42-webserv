//
// Created by Quentin Robert de beauchamp on 1/13/22.
//

#include "global.hpp"

int main() {
	int server_socket;
	struct sockaddr_in address;

    create_socket(server_socket, address);
	receiving_information(server_socket, address);
	return 0;
}
