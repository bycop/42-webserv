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
		cout << "workers: " << workers << endl;

		for (unsigned long i = 0; i < servers.size(); i++) {
			cout << endl;
			cout << "[SERVER " << i << "]" << endl;
			servers[i].print();
		}
	};

	bool mandatory() {
		for (unsigned long i = 0; i < servers.size(); i++)
			if (!servers[i].mandatory(i))
				return (false);
		return (true);
	};
	int workers;
	vector <Server> servers;

private:

};

#endif