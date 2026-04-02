#pragma once

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"
#define NC "\e[0m"

#include <string>
#include <stack>

struct	s_rpnstack
{
	std::stack<int>	rpn;
};

void	parseInput(std::string input, s_rpnstack& stc);
// std::stack<int>	burildStack(std::string input);