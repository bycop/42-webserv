#ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"
# include <map>

using namespace std;

class Server {
public:
	Server() : _autoindex(false), _client_max_body_size(-1), _redirect_status(-1), _port(-1) {};

	~Server() {};

	void print() {
		cout << "host: " << this->_host << endl;
		cout << "ports: " << this->_port << endl;
		cout << "server_names: ";
		for (unsigned long i = 0; i < _server_name.size(); i++)
			cout << _server_name[i] << " ";
		cout << endl;
		cout << "default_pages: " << endl;
		for (map<string, string>::iterator it = _default_pages.begin(); it != _default_pages.end(); ++it)
			cout << "              " << it->first << " " << it->second << endl;
		cout << "client_max_body_size: " << _client_max_body_size << endl;
		cout << "redirect_status: " << _redirect_status << endl;
		cout << "autoindex: " << _autoindex << endl;

		for (unsigned long i = 0; i < _locations.size(); i++) {
			cout << endl;
			cout << "[LOCATION " << i << "]" << endl;
			_locations[i].print();
		}
	};

	bool mandatory() {
		if (_host.empty() || _server_name.empty() || _port == -1)
			return (false);
		return (true);
	}

	bool isAutoindex() const { return _autoindex; }

	const string &getHost() const { return _host; }

	vector<string> &getServerName() { return _server_name; }

	map<string, string> &getDefaultPages() { return _default_pages; }

	int getClientMaxBodySize() const { return _client_max_body_size; }


	const string &getRedirect() const { return _redirect; }

	vector<Location> &getLocations() { return _locations; }

	int &getPort() { return _port; }


	void setAutoindex(bool autoindex) { _autoindex = autoindex; }

	void setHost(const string &host) { _host = host; }

	void setServerName(const vector<string> &serverName) { _server_name = serverName; }

	void setDefaultPages(const map<string, string> &defaultPages) { _default_pages = defaultPages; }

	void setClientMaxBodySize(int clientMaxBodySize) { _client_max_body_size = clientMaxBodySize; }

	void setRedirect(const string &redirect) { _redirect = redirect; }

	void setLocations(const vector<Location> &locations) { _locations = locations; }

	void setPort(int port) { _port = port; }

private:
	bool _autoindex;
	string _host;
	vector<string> _server_name;
	map<string, string> _default_pages;
	int _client_max_body_size;
	int _redirect_status;
	string _redirect;
	vector<Location> _locations;
	int _port;
};

#endif
