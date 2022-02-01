#ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"

class Server {
public:
	Server() {};

	~Server() {};

	void print() {
		std::cout << "host: " << this->host << std::endl;
		std::cout << "port: " << this->port << std::endl;
		std::cout << "server_name: " << this->server_name << std::endl;
		std::cout << "default_pages: " << this->default_pages << std::endl;
		std::cout << "client_max_body_size: " << this->client_max_body_size << std::endl;
		std::cout << "redirect: " << this->redirect << std::endl;
		std::cout << "autoindex: " << this->autoindex << std::endl;

		for (unsigned long i = 0; i < locations.size(); i++)
		{
			std::cout << std::endl;
			std::cout << "[LOCATION " << i << "]" << std::endl;
			locations[i].print();
		}
	};
	bool mandatory(int index = -1) {
		if (host.empty() || server_name.empty() || (index == 0 && port.empty()))
			return (false);
		return (true);
	}

	bool autoindex;
	std::string host; // string
	std::string port; // int[]
	std::string server_name; // string[]
	std::string default_pages; // string
	std::string client_max_body_size; // int
	std::string redirect_status; // 301 : int
	std::string redirect; // https://localhost:8080  string
	std::vector<Location> locations;
};

#endif
