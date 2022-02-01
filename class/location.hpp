#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

using namespace std;

class Location {
public:
	Location() {};

	~Location() {};

	void print() {
		cout << "indexs: ";
		for (unsigned long i = 0; i < index.size(); i++)
			cout << index[i] << " ";
		cout << endl;
		cout << "root: " << this->root << endl;
		cout << "allow_methods: ";
		for (unsigned long i = 0; i < allow_methods.size(); i++)
			cout << allow_methods[i] << " ";
		cout << endl;
		cout << "path: " << this->path << endl;
		cout << "upload_store: " << this->upload_store << endl;
	};
	vector <string> index;
	string root;
	vector <string> allow_methods;
	string path;
	string upload_store;
};

#endif
