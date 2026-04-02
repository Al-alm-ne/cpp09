#include "RPN.hpp"

int	main(int ac, char** av)
{
	if (ac == 2)
	{
		s_rpnstack	stc;
		parseInput(av[1], stc);
	}
	return 0;
}