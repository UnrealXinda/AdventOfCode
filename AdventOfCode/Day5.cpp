#include <iostream>
#include <fstream>
#include <limits>
#include <assert.h>
#include <algorithm>

#include "Day5.h"

namespace
{
	template <class It, class FlagType>
	auto BinarySearch(It start, It end, FlagType lowerFlag, FlagType upperFlag) -> int
	{
		auto min = 0;
		auto max = (1 << std::distance(start, end)) - 1;

		for (;start != end; ++start)
		{
			auto halfDiff = (max - min) >> 1;
			min += (*start == upperFlag) * halfDiff + (*start == upperFlag);
			max -= (*start == lowerFlag) * halfDiff + (*start == lowerFlag);
		}

		assert(min == max);
		return min;
	}
}


void Day5::SolutionPartOne(const std::string& inputFilePath)
{
	const auto kSeatStrLength = 10;
	const auto kRowStrLength = 7;
	const auto kRowSeatCount = 1 << (kSeatStrLength - kRowStrLength);
	auto input = std::ifstream(inputFilePath);
	auto max = 0;

	for (std::string line; std::getline(input, line); )
	{
		assert(line.length() == kSeatStrLength);
		auto rowNum = BinarySearch(line.cbegin(), line.cbegin() + kRowStrLength, 'F', 'B');
		auto colNum = BinarySearch(line.cbegin() + kRowStrLength, line.cend(), 'L', 'R');

		max = std::max(max, rowNum * kRowSeatCount + colNum);
	}

	std::cout << "Max seat ID is: " << max << std::endl;
}

void Day5::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto kSeatStrLength = 10;
	const auto kRowStrLength = 7;
	const auto kRowCount = 1 << kRowStrLength;
	const auto kRowSeatCount = 1 << (kSeatStrLength - kRowStrLength);
	auto input = std::ifstream(inputFilePath);
	auto min_idx = std::numeric_limits<int>::max();
	auto max_idx = std::numeric_limits<int>::min();
	auto count = 0;
	auto sum = 0;

	for (std::string line; std::getline(input, line); ++count)
	{
		assert(line.length() == kSeatStrLength);
		auto rowNum = BinarySearch(line.cbegin(), line.cbegin() + kRowStrLength, 'F', 'B');
		auto colNum = BinarySearch(line.cbegin() + kRowStrLength, line.cend(), 'L', 'R');
		auto idx = rowNum * kRowSeatCount + colNum;
		min_idx = std::min(min_idx, idx);
		max_idx = std::max(max_idx, idx);
		sum += idx;
	}

	auto missing_idx = (max_idx + min_idx) * (count + 1) / 2 - sum;

	std::cout << "Missing seat ID is: " << missing_idx << std::endl;
}

