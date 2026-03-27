#include "BitcoinExchange.hpp"

std::map<std::string, double> buildDataMap()
{
    std::map<std::string, double>	_data;
	std::ifstream	file("data.csv");
	std::string	line;

	std::getline(file, line);//skip first line
	while(std::getline(file, line))// lê de file e armazena no buffer (line)
	{
		std::istringstream ss(line);//cria um stream de leitura em memória a partir da string (line)
		std::string	date, rate;

		std::getline(ss, date, ',');
		std::getline(ss, rate);
		_data[date] = std::stod(rate);
	}
	return _data;
}

void	printMap(std::map<std::string, double> _data)
{
	for (std::map<std::string, double>::iterator it = _data.begin(); it != _data.end(); it++)
		std::cout << it->first << ", " << it->second << "\n";
}