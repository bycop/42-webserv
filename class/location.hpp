#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

class Location {
public:
	Location() {};

	~Location() {};

	void print() {
		std::cout << "index: " << this->index << std::endl;
		std::cout << "root: " << this->root << std::endl;
		std::cout << "default_pages: " << this->allow_methods << std::endl;
		std::cout << "path: " << this->path << std::endl;
	};
	std::string index; // string[]
	std::string root; // string
	std::string allow_methods; // string[]
	std::string path; // string
};

#endif
