#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include "location.hpp"

class Server {
public:
	Server() {};

	~Server() {};

	void print() {
		std::cout << "host: " << this->host << std::endl;
		std::cout << "port: " << this->port << std::endl;
		std::cout << "server_name: " << this->server_name << std::endl;
		std::cout << "default_pages: " << this->default_pages << std::endl;
		std::cout << "size_limit: " << this->size_limit << std::endl;
		std::cout << "redirect: " << this->redirect << std::endl;

		for (int i = 0; i < locations.size(); i++)
		{
			std::cout << std::endl;
			std::cout << "[LOCATION " << i << "]" << std::endl;
			locations[i].print();
		}
	};

	std::string host;
	std::string port;
	std::string server_name;
	std::string default_pages;
	std::string size_limit;
	std::string redirect;
	std::vector<Location> locations;
};

#endif
