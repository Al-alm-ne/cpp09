#include "BitcoinExchange.hpp"
#include <iostream>

int	main(int ac, char** av)
{
	if (ac == 2)
	{
		std::map<std::string, double> _data;
		_data = buildDataMap();
		parseData(av[1], _data);
	}
	else
		std::cout << BRED"Error: could not open file.\n" NC;
	return 0;
}