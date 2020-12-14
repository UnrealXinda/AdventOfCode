#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "Day13.h"

namespace
{
	template <class T>
	auto GetMinTimesDiffAbove(T factor, T target) -> T
	{
		return (static_cast<T>(std::floor((target - 0.01 * factor) / factor)) + 1) * factor - target;
	}

	template <class T>
	void ExtendedEuclideanAlgorithm(T a, T b, T& x, T& y)
	{
		// a * x + b * y = GCD(a, b)
		if (b == 0)
		{
			x = 1;
			y = 0;
		}
		else
		{
			T x1, y1;
			ExtendedEuclideanAlgorithm(b, a % b, x1, y1);

			auto sign = (a * b > 0) ? 1 : -1;
			x = sign * y1;
			y = sign * x1 - sign * y1 * (a / b);
		}
	}

	template <class T, class It>
	auto ChineseRemainderTheorem(const It& numBegin, const It& numEnd, const It& remainderBegin, const It& remainderEnd, int count) -> T
	{
		auto result = static_cast<T>(0);

		auto lcm = std::accumulate(numBegin, numEnd, static_cast<T>(1), [](auto product, const auto& num)
		{
			return product * num;
		});

		auto numIt = numBegin;
		auto remainderIt = remainderBegin;

		for (; numIt != numEnd && remainderIt != remainderEnd; ++numIt, ++remainderIt)
		{
			auto x = static_cast<T>(0);
			auto y = static_cast<T>(0);
			ExtendedEuclideanAlgorithm<T>(lcm / (*numIt), -(*numIt), x, y);
			result += (*numIt * y + 1) * (*remainderIt);
		}

		result %= lcm;
		return result;
	}
}

void Day13::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream{ inputFilePath };
	auto line = std::string();
	auto busIds = std::vector<int>();

	std::getline(input, line);
	auto timestamp = std::stoi(line);

	std::getline(input, line);
	auto stream = std::stringstream(line);

	for (auto subStr = std::string(); std::getline(stream, subStr, ','); )
	{
		if (subStr.compare("x") != 0)
		{
			busIds.push_back(std::stoi(subStr));
		}
	}

	auto diffs = std::vector<std::pair<int, int>>();
	std::transform(std::cbegin(busIds), std::cend(busIds), std::back_inserter(diffs),
	[timestamp](auto id) -> std::pair<int, int>
	{
		auto diff = GetMinTimesDiffAbove(id, timestamp);
		return {id, diff};
	});

	auto min = std::min_element(std::cbegin(diffs), std::cend(diffs),
	[](const auto& a, const auto& b)
	{
		return a.second < b.second;
	});

	std::cout << min->first << " * " << min->second << " = " << min->first * min->second << std::endl;
}

void Day13::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream{ inputFilePath };
	auto line = std::string();
	auto nums = std::vector<int>();
	auto remainders = std::vector<int>();

	std::getline(input, line);
	auto timestamp = std::stoi(line);

	std::getline(input, line);
	auto stream = std::stringstream(line);

	auto idx = 0;
	for (auto subStr = std::string(); std::getline(stream, subStr, ','); ++idx)
	{
		if (subStr.compare("x") != 0)
		{
			auto num = std::stoi(subStr);
			nums.emplace_back(num);
			remainders.emplace_back(idx);
		}
	}

	std::transform(remainders.begin(), remainders.end(), remainders.begin(),
	[idx](auto remainder) -> int
	{
		return remainder - idx + 1;
	});

	auto result = ChineseRemainderTheorem<long long>(nums.cbegin(), nums.cend(), remainders.cbegin(), remainders.cend(), idx);
	std::cout << "The least number that satisfies: " << std::abs(result) - idx + 1 << std::endl;
}