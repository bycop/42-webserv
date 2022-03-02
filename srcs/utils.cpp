//
// Created by Alexis Lafrance on 2/8/22.
//

#include "webserv.hpp"

void display_banner() {
	cout << "██     ██  █████  ██████  ███    ██ ██ ███    ██  ██████              ███████ ███████ ██████  ██    ██ ███████ ██████  \n" <<
		 	"██     ██ ██   ██ ██   ██ ████   ██ ██ ████   ██ ██                   ██      ██      ██   ██ ██    ██ ██      ██   ██ \n" <<
		 	"██  █  ██ ███████ ██████  ██ ██  ██ ██ ██ ██  ██ ██   ███             ███████ █████   ██████  ██    ██ █████   ██████  \n" <<
		 	"██ ███ ██ ██   ██ ██   ██ ██  ██ ██ ██ ██  ██ ██ ██    ██                  ██ ██      ██   ██  ██  ██  ██      ██   ██ \n" <<
			 " ███ ███  ██   ██ ██   ██ ██   ████ ██ ██   ████  ██████              ███████ ███████ ██   ██   ████   ███████ ██   ██ \n";
}

string atoiString(int nb) {
	string str;
	stringstream ss;

	ss << nb;
	ss >> str;
	return (str);
}

string splitPartsByParts(string const& line, const char delimiter, size_t *start) {
	size_t end;
	string part;

	if (*start > line.length())
		return (part);
	end = line.find(delimiter, *start);
	if (end == string::npos) {
		part = line.substr(*start, line.length() - *start);
		*start = line.length() + 1;
		return (part);
	}
	part = line.substr(*start, end - *start);
	*start = end + 1;
	return (part);
}

string	readFile(std::string const& filename) {
	std::ifstream ifs(filename);
	int length;
	char *buf;

	if (!ifs.is_open()) {
		std::cout << "ERROR FILESTREAM" << std::endl;
		return (string());
	}
	ifs.seekg(0, ifs.end);
	length = ifs.tellg();
	buf = new char[length + 1];
	ifs.seekg(0, ifs.beg);
	ifs.read(buf, length);
	buf[length] = '\0';
	ifs.close();
	string tmp(buf);
	delete [] buf;
	return (tmp);
}

bool endsWith(const string &str, const string &suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

bool startsWith(const string &str, const string &prefix)
{
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

int		ft_whitespace(char c)
{
	if (c == '\n' || c == '\t' || c == '\v' || \
			c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int				ft_check_error(const char *base)
{
	int			i;
	int			j;

	i = 0;
	if (strlen(base) == 0 || strlen(base) == 1)
		return (0);
	while (base[i])
	{
		j = i + 1;
		if (base[i] == '+' || base[i] == '-' || ft_whitespace(base[i]))
			return (0);
		while (base[j])
		{
			if (base[i] == base[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}


static int	ft_is_in_base(char c, const char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int			ft_atoi_base(const char *str, const char *base)
{
	int		signe;
	int		n;
	int		i;
	int		base_l;

	i = 0;
	signe = 1;
	n = 0;
	base_l = strlen(base);
	if (!(ft_check_error(base)))
		return (0);
	while (ft_whitespace(str[i]))
		i++;
	while (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			signe = -signe;
	while (str[i] && (ft_is_in_base(str[i], base) != -1))
		n = n * base_l + ft_is_in_base(str[i++], base);
	return (n * signe);
}

const char *detectBase(const string &len) {
	string hex = "abcdef";
	string hex_c = "ABCDEF";
	for (string::const_iterator it = len.begin(); it != len.end(); it++) {
		for (string::iterator it2 = hex.begin(); it2 != hex.end(); it2++) {
			if (*it == *it2)
				return ("0123456789abcdef");
		}
		for (string::iterator it3 = hex_c.begin(); it3 != hex_c.end(); it3++) {
			if (*it == *it3)
				return ("0123456789ABCDEF");
		}
	}
	return ("0123456789");
}

void removeWS(string &line, size_t &pos_del) {
	if (line.size() > pos_del + 1 && pos_del < line.size() && line[pos_del + 1] == ' ')
		line.erase(pos_del + 1, 1);
	pos_del = line.find(':');
}