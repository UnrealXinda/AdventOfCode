#include <iostream>
#include <fstream>
#include <limits>
#include <assert.h>
#include <vector>
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
	auto seats = std::vector<bool>(kRowCount * kRowSeatCount, false);

	for (std::string line; std::getline(input, line); )
	{
		assert(line.length() == kSeatStrLength);
		auto rowNum = BinarySearch(line.cbegin(), line.cbegin() + kRowStrLength, 'F', 'B');
		auto colNum = BinarySearch(line.cbegin() + kRowStrLength, line.cend(), 'L', 'R');
		seats[rowNum * kRowSeatCount + colNum] = true;
	}

	auto first = std::find(seats.cbegin(), seats.cend(), true);
	auto last = std::find(seats.crbegin(), seats.crend(), true).base();
	auto missing = std::find(first, last, false);
	auto index = std::distance(seats.cbegin(), missing);

	std::cout << "Missing seat ID is: " << index << std::endl;
}

