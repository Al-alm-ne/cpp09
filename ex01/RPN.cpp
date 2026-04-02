#include "RPN.hpp"

#include <cstdlib>
#include <string>
#include <iostream>

bool	is_operator(char c)
{
	if (c == '+' || c == '-' || c == '/' || c == '*')
		return true;
	return false;
}

// std::stack<int>	buildStack(std::string input)
// {

// }

void	parseInput(std::string input, s_rpnstack& stc)
{
	int	count = 0;

	for (size_t i = 0; i != input.length(); ++i)
	{
		if (isdigit(input[i]))
		{
			if (i + 1 < input.length() && !isspace(input[i + 1]))
			{
				std::cerr << BRED"Error: invalid input.\n" NC;
				exit(EXIT_FAILURE);
			}
			stc.rpn.push(input[i] - '0');
			count++;
		}
		else if (is_operator(input[i]))
		{
			if ((i + 1 < input.length() && !isspace(input[i + 1])) || count < 2)
			{
				std::cerr << BRED"Error: invalid input.\n" NC;
				exit(EXIT_FAILURE);
			}
			count--;
		}
		else if (isspace(input[i]))
			continue;
		else
		{
			std::cerr << BRED"Error: invalid input.\n" NC;
			exit(EXIT_FAILURE);
		}
		if (count < 1)
		{
			std::cerr << BRED"Error: invalid input.\n" NC;
			exit(EXIT_FAILURE);
		}
	}
	if (count != 1)
	{
		std::cerr << BRED"Error: invalid input.\n" NC;
		exit(EXIT_FAILURE);
	}
	std::cout << BCYN"DEU CERTO\n" NC;
}