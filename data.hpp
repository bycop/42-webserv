#ifndef DATA_HPP
# define DATA_HPP

# include <iostream>
# include <string>
# include "server.hpp"
# include <vector>

class Data
{

public:
	Data(){};
	~Data(){};
	void print() {
		std::cout << std::endl;
		std::cout << "[BASE]" << std::endl;
		std::cout << "user: " << user << std::endl;
		std::cout << "pid: " << pid << std::endl;
		std::cout << "error_log: " << error_log << std::endl;

		for (int i = 0; i < servers.size(); i++)
		{
			std::cout << std::endl;
			std::cout << "[SERVER " << i << "]" << std::endl;
			servers[i].print();
		}
	};
	std::string user;
	std::string pid;
	std::string error_log;
	std::vector<Server> servers;

private:

};

#endif