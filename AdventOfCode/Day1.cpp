#include <iostream>
#include <fstream>
#include <optional>
#include <tuple>
#include <unordered_set>
#include <algorithm>

#include "Day1.h"

namespace
{
	template <class T, class Iter>
	auto GetSumPair(Iter begin, Iter end, T sum) -> std::optional<std::tuple<T, T>>
	{
		auto set = std::unordered_set<T>();
		auto it = std::find_if(begin, end, [&](const auto& i) mutable -> bool
		{
			auto found = set.find(sum - i) != set.cend();
			set.insert(i);
			return found;
		});

		if (it != end)
		{
			return std::make_tuple(*it, sum - *it);
		}

		return std::nullopt;
	}
}

void Day1::SolutionPartOne(const std::string& inputFilePath, int sum)
{
	auto input = std::ifstream(inputFilePath);
	auto nums = std::vector<int>();

	for (std::string line; std::getline(input, line); )
	{
		nums.push_back(std::stoi(line));
	}

	if (auto it = GetSumPair(nums.cbegin(), nums.cend(), sum))
	{
		auto [first, second] = *it;
		std::cout << first << " + " << second << " = " << sum << std::endl;
		std::cout << first << " * " << second << " = " << first * second << std::endl;
	}
}

void Day1::SolutionPartTwo(const std::string& inputFilePath, int sum)
{
	auto input = std::ifstream(inputFilePath);
	auto nums = std::vector<int>();

	for (std::string line; std::getline(input, line); )
	{
		nums.push_back(std::stoi(line));
	}

	for (auto it = nums.cbegin(); it != nums.cend(); ++it)
	{
		if (auto innerIt = GetSumPair(it + 1, nums.cend(), sum - *it))
		{
			auto [first, second] = *innerIt;
			std::cout << *it << " + " << first << " + " << second << " = " << sum << std::endl;
			std::cout << *it << " * " << first << " * " << second << " = " << *it * first * second << std::endl;

			return;
		}
	}
}