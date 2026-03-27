#include "BitcoinExchange.hpp"

int	main()
{
	std::map<std::string, double> _data;
	_data = buildDataMap();
	printMap(_data);
	if (!parseData())
		return 1;
	return 0;
}