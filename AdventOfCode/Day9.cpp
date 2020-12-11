#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include "Day9.h"

namespace
{
	template <class NumType>
	class NumCounter
	{
	public:
		inline void AddNumber(NumType num)
		{
			++m_map[num];
		}

		inline void RemoveNumber(NumType num)
		{
			if (m_map.find(num) != m_map.end())
			{
				--m_map[num];
				if (m_map[num] <= 0)
				{
					m_map.erase(num);
				}
			}
		}

		inline auto GetNumCount(NumType num) const -> unsigned int
		{
			if (m_map.find(num) == m_map.end())
			{
				return 0;
			}
			return m_map.at(num);
		}

		inline auto cbegin() const { return m_map.cbegin(); }
		inline auto cend() const { return m_map.cend(); }

	private:
		std::unordered_map<NumType, unsigned int> m_map;
	};


	template <class NumType>
	auto CanSumUpTo(const NumCounter<NumType>& counter, NumType sum) -> bool
	{
		return std::any_of(counter.cbegin(), counter.cend(), [&](const auto& kvp) -> bool
		{
			return counter.GetNumCount(sum - kvp.first) > ((2 * kvp.first == sum) ? 1 : 0);
		});
	}
}

void Day9::SolutionPartOne(const std::string& inputFilePath)
{
	const int kPremableCount = 25;
	auto input = std::ifstream(inputFilePath);
	auto nums = std::vector<unsigned long long>();
	auto counter = NumCounter<unsigned long long>();

	for (auto line = std::string(); std::getline(input, line); )
	{
		nums.push_back(std::stoull(line));
	}

	for (auto i = 0; i < kPremableCount; ++i)
	{
		counter.AddNumber(nums[i]);
	}

	for (auto i = kPremableCount; i < nums.size(); ++i)
	{
		if (!CanSumUpTo(counter, nums[i]))
		{
			std::cout << "The first number that can not be summed up to by previous "
			          << kPremableCount  << " numbers is " << nums[i] << std::endl;
			return;
		}

		counter.RemoveNumber(nums[i - kPremableCount]);
		counter.AddNumber(nums[i]);
	}
}

void Day9::SolutionPartTwo(const std::string& inputFilePath)
{
	const int kPremableCount = 25;
	auto input = std::ifstream(inputFilePath);
	auto nums = std::vector<unsigned long long>();
	auto counter = NumCounter<unsigned long long>();
	auto invalidNum = 0ULL;

	for (auto line = std::string(); std::getline(input, line); )
	{
		nums.push_back(std::stoull(line));
	}

	for (auto i = 0; i < kPremableCount; ++i)
	{
		counter.AddNumber(nums[i]);
	}

	for (auto i = kPremableCount; i < nums.size(); ++i)
	{
		if (!CanSumUpTo(counter, nums[i]))
		{
			invalidNum = nums[i];
			nums.erase(nums.begin() + i);
			break;
		}

		counter.RemoveNumber(nums[i - kPremableCount]);
		counter.AddNumber(nums[i]);
	}

	auto startIt = nums.cbegin();
	auto endIt = nums.cbegin();
	auto partialSum = 0ULL;
	
	while (endIt != nums.cend())
	{
		if (partialSum < invalidNum)
		{
			partialSum += *(endIt++);
		}
		else if (partialSum > invalidNum)
		{
			partialSum -= *(startIt++);
		}
		else
		{
			// Avoid getting just the invalid number
			if (startIt != endIt)
			{
				break;
			}
		}

		if (startIt == endIt)
		{
			++endIt;
		}
	}

	assert(startIt != nums.cend());
	assert(endIt != nums.cend());
	assert(startIt != endIt);

	auto min = *std::min_element(startIt, endIt);
	auto max = *std::max_element(startIt, endIt);
	std::cout << "Min number: " << min << " Max number: " << max << " Sum: " << min + max << std::endl;
}
