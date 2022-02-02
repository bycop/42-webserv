#ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"

using namespace std;

class Server {
public:
	Server() : autoindex(false), client_max_body_size(0), redirect_status(0) {};

	~Server() {};

	void print() {
		cout << "host: " << this->host << endl;
		cout << "ports: ";
		for (unsigned long i = 0; i < ports.size(); i++)
			cout << ports[i] << " ";
		cout << endl;
		cout << "server_names: ";
		for (unsigned long i = 0; i < server_name.size(); i++)
			cout << server_name[i] << " ";
		cout << endl;
		cout << "default_pages: " << this->default_pages << endl;
		cout << "client_max_body_size: " << this->client_max_body_size << endl;
		cout << "redirect_status: " << this->redirect_status << endl;
		cout << "redirect: " << this->redirect << endl;
		cout << "autoindex: " << this->autoindex << endl;

		for (unsigned long i = 0; i < locations.size(); i++) {
			cout << endl;
			cout << "[LOCATION " << i << "]" << endl;
			locations[i].print();
		}
	};

	bool mandatory(int index = -1) {
		if (host.empty() || server_name.empty() || (index == 0 && ports.empty()))
			return (false);
		return (true);
	}

	bool autoindex;
	string host;
	vector <string> server_name;
	string default_pages;
	int client_max_body_size;
	int redirect_status;
	string redirect;
	vector <Location> locations;
	vector <int> ports;
};

#endif
