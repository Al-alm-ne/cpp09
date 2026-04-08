#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

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
		_data[date] = std::strtod(rate.c_str(), NULL);
	}
	return _data;
}

bool	parseDate(const std::string& date)
{
	if (date.length() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;
	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (date[i] < '0' || date[i] > '9')
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	if ((month < 1 || month > 12) || (year < 2009 || year > 2022))
		return false;

	int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	bool leapMonth = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (leapMonth)
		daysInMonth[1] = 29;
	if (day < 1 || day > daysInMonth[month - 1])
		return false;
	return true;
}

bool	parseValue(const std::string& value)
{
	size_t	i = 0;

	if (value[0] == '-')
		i++;
	int	count = 0;

	while (i < value.length())
	{
		if (value[i] == '.')
			count++;
		if (count > 1 || value[0] == '.' || value[value.length() -1] == '.')
		{
			std::cout << BRED"Error: invalid value.\n" NC;
			return false;
		}
		if ((value[i] < '0' || value[i] > '9') && value[i] != '.')
		{
			std::cout << BRED"Error: invalid value.\n" NC;
			return false;
		}
		i++;
	}
	double dvalue = std::strtod(value.c_str(), NULL);

	if (dvalue < 0)
	{
		std::cout << BRED"Error: not a positive number.\n" NC;
		return false;
	}
	if (dvalue > 1000)
	{
		std::cout << BRED"Error: too large a number.\n" NC;
		return false;
	}
	return true;
}

void	calculate(std::string date, std::string value, std::map<std::string, double> _data)
{
	std::map<std::string, double>::const_iterator it = _data.lower_bound(date);// retorna a referência para a chave exata, se existir, ou para a chave imediatamente superior, se não existir, ou end() se todas forem < date.

	if (it == _data.end() || it->first != date)// não encontrou a data exata
	{
		if (it == _data.begin())// data inferior a primeira.
		{
			std::cout << BRED"Error: no exchange rate available for this date.\n" NC;
			return;
		}
		--it;// move a referência para a data inferior.
	}
	std::cout << date << " => " << value << " = " << std::strtod(value.c_str(), NULL) * it->second << "\n";
}

void	parseData(const std::string& input, std::map<std::string, double> _data)
{
	std::ifstream	file(input.c_str());

	if (!file.good())
	{
		std::cout << BRED"Error: could not open file.\n" NC;
		return;
	}
	std::string	firstline;
	std::getline(file, firstline);

	if(firstline.empty())
	{
		std::cout << BRED"Error: no firstline.\n" NC;
		return;
	}

	if(firstline.compare("date | value"))
	{
		std::cout << BRED"Error: incorrect first line => " BMAG << firstline << "\n" NC;
		return;
	}
	std::string	line;

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
	 	std::string date, value;
		std::string extra;
		char	sep;

		if (!(ss >> date >> sep >> value) || sep != '|' || line.length() < 13 || line[12] != ' ' || line[13] == ' ' || (ss >> extra))//extrai 3 tokens de line (date, sep e value). Precisa de espaco entre os elementos.
		{
			std::cout << BRED"Error: bad input => " BMAG << line << "\n" NC;
			continue;
		}
		if (!parseDate(date))
		{
			std::cout << BRED"Error: invalid date => " BMAG << date << "\n" NC;
			continue;
		}
		if (!parseValue(value))
		{
		 	continue;
		}
		// std::cout << line << "\n";
		calculate(date, value, _data);
	}
	file.close();
}