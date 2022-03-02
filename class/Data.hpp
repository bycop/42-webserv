#ifndef DATA_HPP
# define DATA_HPP

# include "Server.hpp"

using namespace std;

class Data {
public:
	Data() : _isRunning(true) {};

	~Data() {};

	void print() {
		cout << endl;
		cout << "[BASE]" << endl;

		for (unsigned long i = 0; i < _servers.size(); i++) {
			cout << endl;
			cout << "[SERVER " << i << "]" << endl;
			_servers[i].print();
		}
	};

	bool mandatory() {
		if (_servers.empty()) {
			cerr << "\e[91m[ERROR]\e[0m Mandatory key is missing " << endl;
			return (false);
		}
		for (unsigned long i = 0; i < _servers.size(); i++)
			if (!_servers[i].mandatory()) {
				cerr << "\e[91m[ERROR]\e[0m Mandatory key is missing " << endl;
				return (false);
			}
		for (unsigned long i = 0; i < _servers.size(); i++)
			for (unsigned long j = 0; j < _servers.size(); j++)
				if (i != j && _servers[i].getPort() == _servers[j].getPort() &&
					_servers[i].getHost() == _servers[j].getHost() &&
					alreadyExistSN(_servers[i].getServerName(), _servers[j].getServerName()))
					return (false);

		return (true);
	}

	vector<Server> &getServers() { return _servers; }

	void setServers(const vector<Server> &servers) { _servers = servers; }

	bool IsRunning() const {
		return _isRunning;
	}

	void setIsRunning(bool isRunning) {
		_isRunning = isRunning;
	};

	bool checkFdAlreadyAccepted(int fd) {
		for (vector<int>::iterator it = _socket_fd.begin(); it < _socket_fd.end(); ++it) {
			if (*it == fd)
				return true;
		}
		return false;
	}

	void pushSocketFdAccepted(int fd) {
		for (vector<int>::iterator it = _socket_fd.begin(); it < _socket_fd.end(); ++it) {
			if (*it == fd) {
				cerr << "FD already accepted" << endl;
				return ;
			}
		}
		_socket_fd.push_back(fd);
	}

	const vector<int> &getSocketFdAccepted() const {
		return _socket_fd;
	}

	void setSocketFdAccepted(const vector<int> &socket_fd) {
		this->_socket_fd = socket_fd;
	}

	void eraseSocketFd(int fd) {
		for (vector<int>::iterator it = _socket_fd.begin(); it < _socket_fd.end(); ++it) {
			if (*it == fd) {
				_socket_fd.erase(it);
				return ;
			}
		}
	}

private:
	static bool alreadyExistSN(vector<string> sn1, vector<string> sn2) {
		for (unsigned long i = 0; i < sn1.size(); i++)
			for (unsigned long j = 0; j < sn2.size(); j++)
				if (sn1[i] == sn2[j])
					return (true);
		return (false);
	}

	vector<int>	_socket_fd;
	vector<Server> _servers;
	bool _isRunning;
};

#endif