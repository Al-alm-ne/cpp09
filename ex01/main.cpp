#include "RPN.hpp"
#include <iostream>

int main(int ac, char **av)
{
	if (ac == 2)
	{
		s_rpnstack stc;
		parseInput(av[1], stc);
	}
	else
	{
		std::cout << BRED << "Error.\n"
				  << NC;
	}
	return 0;
}