#include <iostream>
#include <fstream>
#include <iterator>
#include <array>
#include <unordered_set>
#include <algorithm>
#include "Day6.h"

namespace
{
	void AppendUniqueQuestions(std::unordered_set<char>& set, const std::string& str)
	{
		std::transform(str.cbegin(), str.cend(), std::inserter(set, set.begin()),
		[&set](const auto ch) -> char { return ch; });
	}

	template <class Array>
	void AggregateAnswer(Array& arr, const std::string& str)
	{
		std::for_each(str.cbegin(), str.cend(), [&arr](const auto ch) { ++arr[ch - 'a']; });
	}

	template <class Array>
	auto GetIntersectedAnswerCount(const Array& arr, int personCount) -> std::size_t
	{
		return std::count(std::cbegin(arr), std::cend(arr), personCount);
	}
}

void Day6::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto set = std::unordered_set<char>();
	auto count = 0;

	for (std::string line; std::getline(input, line); )
	{
		if (!line.empty())
		{
			AppendUniqueQuestions(set, line);
		}

		else
		{
			count += set.size();
			set.clear();
		}
	}

	count += set.size();

	std::cout << "Total questions answered: " << count << std::endl;
}

void Day6::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto strs = std::vector<std::string>();
	auto charCount = std::array<std::size_t, 26>();
	auto count = 0;
	auto personCount = 0;

	charCount.fill(0);

	for (std::string line; std::getline(input, line); )
	{
		if (!line.empty())
		{
			AggregateAnswer(charCount, line);
			++personCount;
		}

		else
		{
			count += GetIntersectedAnswerCount(charCount, personCount);
			std::fill(charCount.begin(), charCount.end(), 0);
			personCount = 0;
		}
	}

	count += GetIntersectedAnswerCount(charCount, personCount);

	std::cout << "Total intersected questions answered: " << count << std::endl;

}

