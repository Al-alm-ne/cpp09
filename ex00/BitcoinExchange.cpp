#include "BitcoinExchange.hpp"
#include <fstream>
#include <string>

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

bool	parseData()
{
	std::ifstream	file("input.txt");

	if(!file)
	{
		std::cout << BRED"Input file does not exist!\n" NC;
		return false;
	}
	std::string	firstline;
	std::getline(file, firstline);

	if(firstline.compare("date | value"))
	{
		std::cout << BRED"Incorrect first line => " BMAG << firstline << "\n" NC;
		return false;
	}
	std::string	line;
	
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
	 	std::string date, value;
		char	sep;

		if (!(ss >> date >> sep >> value) || sep != '|')//extrai 3 tokens de line (date, sep e value). Precisa de espaco entre os elementos.
		{
			std::cout << BRED"Bad Input => " BMAG << line << "\n" NC;
			return false;
		}
/* 		else if (!parseDate(date))
		{
			std::cout << BRED"Invalid Date => " BMAG << date << "\n" NC;
			return false;
		}
		else if (!parseValue(value))
		{
			std::cout << BRED"Invalid Value => " BMAG << value << "\n" NC;
			return false;
		} */
		else
		{
			std::cout << BRED" Invalid Input!\n" NC;
			return false;
		}
	}
	file.close();
	return true;
}