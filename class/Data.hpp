#ifndef DATA_HPP
# define DATA_HPP

# include "Server.hpp"

using namespace std;

class Data {
public:
	Data() {};

	~Data() {};

	void print() {
		cout << endl;
		cout << "[BASE]" << endl;
		cout << "workers: " << _workers << endl;

		for (unsigned long i = 0; i < _servers.size(); i++) {
			cout << endl;
			cout << "[SERVER " << i << "]" << endl;
			_servers[i].print();
		}
	};

	bool mandatory() {
		for (unsigned long i = 0; i < _servers.size(); i++)
			if (!_servers[i].mandatory(i))
				return (false);
		return (true);
	}

	vector<Server> &getServers() { return _servers; }

	int getWorkers() const { return _workers; }


	void setServers(const vector<Server> &servers) { _servers = servers; }

	void setWorkers(int workers) { _workers = workers; };

private:

	vector<Server> _servers;
	int _workers;

};

#endif