#include <fstream>

#include "data.hpp"

void setters(std::string newdata, int type, Data *data) {
  switch (type) {
    case 0:
      data->user = newdata;
      break;
    case 1:
      data->pid = newdata;
      break;
    case 2:
      data->error_log = newdata;
      break;
  }
}

void server_setters(std::string newdata, int type, Server *server) {
  switch (type) {
    case 0:
      server->host = newdata;
      break;
    case 1:
      server->port = newdata;
      break;
    case 2:
      server->server_name = newdata;
      break;
    case 3:
      server->default_pages = newdata;
      break;
    case 4:
      server->size_limit = newdata;
      break;
  }
}

void location_setters(std::string newdata, int type, Location *location) {
  switch (type) {
    case 0:
      if (newdata == "on")
        location->autoindex = true;
      else
        location->autoindex = false;
      break;
    case 1:
      location->index = newdata;
      break;
    case 2:
      location->root = newdata;
      break;
    case 3:
      location->allow_methods = newdata;
      break;
  }
}

void location_loop(std::ifstream &file, Server *server) {
  Location location;
  std::string line;
  std::string locationvars[4] = {"autoindex", "index", "root", "allow_methods"};
  bool find;

  while (std::getline(file, line)) {
    std::string original = line;

    find = false;
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    for (int i = 0; i < 5; i++)
      if (line.find(locationvars[i] + ":", 0) == 0) {
        line = &line[locationvars[i].length() + 1];
        location_setters(line, i, &location);
        find = true;
      }
    if (line.find("}", 0) == 0)
      break;
    else if (!find && line != "")
      std::cout << "\e[93m[WARNING]\e[0m Unrecognized location key at '"
                << original << "'" << std::endl;
  }

  server->locations.push_back(location);
}

void server_loop(std::ifstream &file, Data *data) {
  Server server;
  std::string line;
  std::string servervars[5] = {"host", "port", "server_name", "default_pages",
                               "size_limit"};
  bool find;

  while (std::getline(file, line)) {
    std::string original = line;

    find = false;
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    for (int i = 0; i < 5; i++)
      if (line.find(servervars[i] + ":", 0) == 0) {
        line = &line[servervars[i].length() + 1];
        server_setters(line, i, &server);
        find = true;
      }
    if (line.find("}", 0) == 0) break;
    if (!find && line.find("location{", 0) == 0)
      location_loop(file, &server);
    else if (!find && line != "")
      std::cout << "\e[93m[WARNING]\e[0m Unrecognized server key at '"
                << original << "'" << std::endl;
  }

  data->servers.push_back(server);
}

int main(int ac, char **av) {
  std::string file_path;
  if (ac == 2) {
    file_path = av[1];
  } else {
    std::cout << "Usage: ./parser <file_path>" << std::endl;
    return (1);
  }

  std::ifstream file(file_path);

  if (!file.is_open()) {
    std::cout << "Error: file not found" << std::endl;
    return (1);
  }

  Data data;
  std::string line;
  bool find;

  std::string vars[3] = {"user", "pid", "error_log"};

  while (std::getline(file, line)) {
    std::string original = line;
    find = false;
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    for (int i = 0; i < 3 && !find; i++)
      if (line.find(vars[i] + ":", 0) == 0) {
        line = &line[vars[i].length() + 1];
        setters(line, i, &data);
        find = true;
      }
    if (!find && line.find("server{", 0) == 0)
      server_loop(file, &data);
    else if (!find && line != "")
      std::cout << "\e[93m[WARNING]\e[0m Unrecognized main key at '" << original
                << "'" << std::endl;
  }

  data.print();

  return (0);
};