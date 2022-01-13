#include "data.hpp"
#include <fstream>

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

int main(int ac, char **av)
{

	std::string file_path;
	if (ac == 2)
	{
		file_path = av[1];
	}
	else
	{
		std::cout << "Usage: ./parser <file_path>" << std::endl;
		return (1);
	}

	std::ifstream file(file_path);

	if (!file.is_open())
	{
		std::cout << "Error: file not found" << std::endl;
		return (1);
	}

	Data *data = new Data();
	std::string line;

	std::string vars[3] = {"user", "pid", "error_log"};

	while (std::getline(file, line))
	{
		for (int i = 0; i < 3; i++)
			if (line.rfind(vars[i], 0) == 0)
			{
				line = &line[vars[i].length()];
				line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
				setters(line, i, data);
			}
	}

	std::cout << "user: " << data->user << std::endl;
	std::cout << "pid: " << data->pid << std::endl;
	std::cout << "error_log: " << data->error_log << std::endl;

	delete data;

	return (0);
};