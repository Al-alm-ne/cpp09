#include "RPN.hpp"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <string>

bool is_operator(char c)
{
	if (c == '+' || c == '-' || c == '/' || c == '*')
		return true;
	return false;
}

std::stack<int> buildstack(std::string input)
{
	std::stack<int> stack;

	for (size_t i = 0; i != input.length(); ++i)
	{
		if (isspace(static_cast<unsigned char>(input[i])))
			continue;
		if (isdigit(static_cast<unsigned char>(input[i])))
		{
			if (i + 1 < input.length() && !isspace(static_cast<unsigned char>(input[i + 1])))
			{
				std::cerr << BRED "Error: invalid input.\n" NC;
				exit(EXIT_FAILURE);
			}
			stack.push(input[i] - '0');
		}
		else if (is_operator(input[i]))
		{
			if (stack.size() < 2 || (i + 1 < input.length() && !isspace(static_cast<unsigned char>(input[i + 1]))))
			{
				std::cerr << BRED "Error: invalid input.\n" NC;
				exit(EXIT_FAILURE);
			}
			int right = stack.top();
			stack.pop();
			int left = stack.top();
			stack.pop();
			int result = 0;

			if (input[i] == '+')
				result = left + right;
			else if (input[i] == '-')
				result = left - right;
			else if (input[i] == '*')
				result = left * right;
			else
			{
				if (right == 0)
				{
					std::cerr << BRED "Error: invalid input.\n" NC;
					exit(EXIT_FAILURE);
				}
				result = left / right;
			}
			stack.push(result);
		}
		else
		{
			std::cerr << BRED "Error: invalid input.\n" NC;
			exit(EXIT_FAILURE);
		}
	}
	if (stack.size() != 1)
	{
		std::cerr << BRED "Error: invalid input.\n" NC;
		exit(EXIT_FAILURE);
	}
	return stack;
}

void parseInput(std::string input, s_rpnstack &stc)
{
	stc.rpn = buildstack(input);
	std::cout << BCYN << stc.rpn.top() << "\n" NC;
}