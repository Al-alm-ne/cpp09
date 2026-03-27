#include "BitcoinExchange.hpp"

int	main()
{
	std::map<std::string, double> _data;
	_data = buildDataMap();
	printMap(_data);
	return 0;
}