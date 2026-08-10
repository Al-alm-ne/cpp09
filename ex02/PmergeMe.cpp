#include "PmergeMe.hpp"

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sys/time.h>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>

namespace
{
	bool isPositiveInteger(const std::string &token, int &value)
	{
		if (token.empty())
			return false;
		char *end = 0;
		long parsed = std::strtol(token.c_str(), &end, 10);
		if (end == token.c_str() || *end != '\0' || parsed <= 0 || parsed > INT_MAX)
			return false;
		value = static_cast<int>(parsed);
		return true;
	}

	std::vector<int> sortWithVector(const std::vector<int> &input)
	{
		std::vector<int> larger;
		std::vector<int> smaller;
		std::vector<int> sorted;

		if (input.empty())
			return sorted;
		for (size_t i = 0; i + 1 < input.size(); i += 2)
		{
			int left = input[i];
			int right = input[i + 1];
			if (left > right)
				std::swap(left, right);
			smaller.push_back(left);
			larger.push_back(right);
		}
		if (input.size() % 2 == 1)
			smaller.push_back(input.back());
		std::sort(larger.begin(), larger.end());
		sorted = larger;
		for (size_t i = 0; i < smaller.size(); ++i)
		{
			int value = smaller[i];
			size_t pos = 0;
			while (pos < sorted.size() && sorted[pos] < value)
				++pos;
			sorted.insert(sorted.begin() + pos, value);
		}
		return sorted;
	}

	std::deque<int> sortWithDeque(const std::vector<int> &input)
	{
		std::deque<int> larger;
		std::deque<int> smaller;
		std::deque<int> sorted;

		if (input.empty())
			return sorted;
		for (size_t i = 0; i + 1 < input.size(); i += 2)
		{
			int left = input[i];
			int right = input[i + 1];
			if (left > right)
				std::swap(left, right);
			smaller.push_back(left);
			larger.push_back(right);
		}
		if (input.size() % 2 == 1)
			smaller.push_back(input.back());
		std::vector<int> temp(larger.begin(), larger.end());
		std::sort(temp.begin(), temp.end());
		sorted.clear();
		for (size_t i = 0; i < temp.size(); ++i)
			sorted.push_back(temp[i]);
		for (size_t i = 0; i < smaller.size(); ++i)
		{
			int value = smaller[i];
			std::deque<int>::iterator it = sorted.begin();
			while (it != sorted.end() && *it < value)
				++it;
			sorted.insert(it, value);
		}
		return sorted;
	}

	void printSequence(const std::string &label, const std::vector<int> &values)
	{
		std::cout << label;
		for (size_t i = 0; i < values.size(); ++i)
		{
			std::cout << (i == 0 ? "" : " ") << values[i];
		}
		std::cout << NC << std::endl;
	}

	double elapsedMicroseconds(const timeval &start, const timeval &end)
	{
		return static_cast<double>((end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec));
	}

}

int PmergeMe::run(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << BRED "Error" NC << std::endl;
		return 1;
	}
	std::vector<int> input;
	for (int i = 1; i < argc; ++i)
	{
		int value = 0;
		if (!isPositiveInteger(argv[i], value))
		{
			std::cerr << BRED "Error" NC << std::endl;
			return 1;
		}
		input.push_back(value);
	}
	printSequence(BMAG "Before: " BWHT, input);

	struct timeval startVector;
	struct timeval endVector;
	struct timeval startDeque;
	struct timeval endDeque;
	gettimeofday(&startVector, 0);
	std::vector<int> sortedVector = sortWithVector(input);
	gettimeofday(&endVector, 0);

	gettimeofday(&startDeque, 0);
	std::deque<int> sortedDeque = sortWithDeque(input);
	gettimeofday(&endDeque, 0);

	printSequence(BCYN "After: " BWHT, sortedVector);
	std::cout << std::fixed << std::setprecision(5);
	std::cout << BMAG "Time to process a range of " << input.size() << " elements with std::vector : " BWHT
			  << elapsedMicroseconds(startVector, endVector) << " us" << NC << std::endl;
	std::cout << BCYN "Time to process a range of " << input.size() << " elements with std::deque : " BWHT
			  << elapsedMicroseconds(startDeque, endDeque) << " us" << NC << std::endl;
	(void)sortedDeque;
	return 0;
}
