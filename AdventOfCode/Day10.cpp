#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include "Day10.h"

namespace
{
	template <class T1, class T2>
	auto CountPossiblePath(const std::vector<T1>& sortedNums, std::vector<T2>& cachedSolutions, int index, T1 prevJolt) -> T2
	{
		if (index >= sortedNums.size())
		{
			return 0;
		}

		if (index == sortedNums.size() - 1)
		{
			return 1;
		}

		if (cachedSolutions[index] >= 0)
		{
			return cachedSolutions[index];
		}

		cachedSolutions[index] = 0;

		if (index + 1 < sortedNums.size())
		{
			cachedSolutions[index] += (sortedNums[index + 1] - sortedNums[index] <= 3) * CountPossiblePath(sortedNums, cachedSolutions, index + 1, sortedNums[index]);
		}

		if (index + 2 < sortedNums.size())
		{
			cachedSolutions[index] += (sortedNums[index + 2] - sortedNums[index] <= 3) * CountPossiblePath(sortedNums, cachedSolutions, index + 2, sortedNums[index]);
		}

		if (index + 3 < sortedNums.size())
		{
			cachedSolutions[index] += (sortedNums[index + 3] - sortedNums[index] <= 3) * CountPossiblePath(sortedNums, cachedSolutions, index + 3, sortedNums[index]);
		}

		return cachedSolutions[index];
	}
}

void Day10::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto nums = std::vector<unsigned int>();
	nums.push_back(0);

	for (auto line = std::string(); std::getline(input, line); )
	{
		nums.push_back(std::stoi(line));
	}

	std::sort(nums.begin(), nums.end(), std::less<unsigned int>());
	std::adjacent_difference(nums.begin(), nums.end(), nums.begin());

	auto oneCount = std::count(nums.cbegin() + 1, nums.cend(), 1);
	auto threeCount = std::count(nums.cbegin() + 1, nums.cend(), 3) + 1;

	std::cout << "Number of 1 jolts: " << oneCount << std::endl;
	std::cout << "Number of 3 jolts: " << threeCount << std::endl;
	std::cout << oneCount << " * " << threeCount << " = " << oneCount * threeCount << std::endl;
}

void Day10::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto nums = std::vector<unsigned int>();
	auto solutions = std::vector<long long>();
	nums.push_back(0);

	for (auto line = std::string(); std::getline(input, line); )
	{
		nums.push_back(std::stoi(line));
	}

	solutions.resize(nums.size());
	std::sort(nums.begin(), nums.end(), std::less<unsigned int>());
	std::fill(solutions.begin(), solutions.end(), -1);

	auto solutionCount = CountPossiblePath<unsigned int, long long>(nums, solutions, 0, 0);
	std::cout << "Possible paths: " << solutionCount << std::endl;
}
