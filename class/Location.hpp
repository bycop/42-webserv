#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

using namespace std;

class Location {
public:
	Location() {};

	Location &operator=(const Location &src) {
		this->_allow_methods = src._allow_methods;
		this->_index = src._index;
		this->_path = src._path;
		this->_root = src._root;
		this->_upload_store = src._upload_store;
		return (*this);
	}

	~Location() {};

	bool isEmpty() {
		if (_index.empty() && _root.length() == 0 && _path.length() == 0 && _upload_store.length() == 0 && _allow_methods.empty())
			return (true);
		return (false);
	}

	void print() {
		cout << "indexs: ";
		for (unsigned long i = 0; i < _index.size(); i++)
			cout << _index[i] << " ";
		cout << endl;
		cout << "root: " << _root << endl;
		cout << "allow_methods: ";
		for (unsigned long i = 0; i < _allow_methods.size(); i++)
			cout << _allow_methods[i] << " ";
		cout << endl;
		cout << "path: " << _path << endl;
		cout << "upload_store: " << _upload_store << endl;
	}

	vector<string> &getIndex() { return _index; }

	const string &getRoot() const { return _root; }

	vector<string> &getAllowMethods() { return _allow_methods; }

	const string &getPath() const { return _path; }

	const string &getUploadStore() const { return _upload_store; }

	void setIndex(const vector<string> &index) { Location::_index = index; }

	void setRoot(const string &root) { Location::_root = root; }

	void setAllowMethods(const vector<string> &allowMethods) { _allow_methods = allowMethods; }

	void setPath(const string &path) { Location::_path = path; }

	void setUploadStore(const string &uploadStore) { _upload_store = uploadStore; };

private:
	vector<string> _index;
	string _root;
	vector<string> _allow_methods;
	string _path;
	string _upload_store;
};

#endif
