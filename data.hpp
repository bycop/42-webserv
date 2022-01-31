#ifndef DATA_HPP
# define DATA_HPP

# include <iostream>
# include <string>
# include "server.hpp"
# include <vector>
# include <algorithm>

class Data
{

public:
	Data(){};
	~Data(){};
	void print() {
		std::cout << std::endl;
		std::cout << "[BASE]" << std::endl;
		std::cout << "workers: " << workers << std::endl;

		for (int i = 0; i < servers.size(); i++)
		{
			std::cout << std::endl;
			std::cout << "[SERVER " << i << "]" << std::endl;
			servers[i].print();
		}
	};
	bool mandatory() {
		for (int i = 0; i < servers.size(); i++)
			if (!servers[i].mandatory(i))
				return (false);
		return (true);
	};
	std::string workers;
	std::vector<Server> servers;

private:

};

#endif