#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Day15.h"

namespace
{
	struct MostRecentCounter
	{
		MostRecentCounter();
		auto GetIndexDifference() const -> unsigned int;
		void AddIndex(unsigned int idx);

		int mostRecentIdx;
		int prevMostRecentIdx;
	};

	MostRecentCounter::MostRecentCounter() :
		mostRecentIdx(-1),
		prevMostRecentIdx(-1)
	{
	}

	auto MostRecentCounter::GetIndexDifference() const -> unsigned int
	{
		return prevMostRecentIdx >= 0 ? mostRecentIdx - prevMostRecentIdx : 0;
	}

	void MostRecentCounter::AddIndex(unsigned int idx)
	{
		prevMostRecentIdx = mostRecentIdx;
		mostRecentIdx = idx;
	}

	template <class T>
	class ElvesNumberGame
	{
	public:
		auto RunGame(const std::vector<T>& inputs, unsigned int turnCount) -> T;
	};

	template <class T>
	auto ElvesNumberGame<T>::RunGame(const std::vector<T>& inputs, unsigned int turnCount) -> T
	{
		auto counterMap = std::unordered_map<T, MostRecentCounter>();
		auto minIdx = std::min(inputs.size(), turnCount);
		auto currentNum = inputs[minIdx - 1];

		for (auto i = 1; i <= minIdx; ++i)
		{
			currentNum = inputs[i - 1];
			counterMap[currentNum].AddIndex(i);
		}

		for (auto i = minIdx + 1; i <= turnCount; ++i)
		{
			currentNum = counterMap[currentNum].GetIndexDifference();
			counterMap[currentNum].AddIndex(i);
		}

		return currentNum;
	}
}

void Day15::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto game = ElvesNumberGame<unsigned int>();
	auto nums = std::vector<unsigned int>();

	for (auto numStr = std::string(); std::getline(input, numStr, ','); )
	{
		nums.push_back(std::stoi(numStr));
	}

	auto result = game.RunGame(nums, 2020);
	std::cout << "Final result after 2020 runs is: " << result << std::endl;
}

void Day15::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto game = ElvesNumberGame<unsigned long long>();
	auto nums = std::vector<unsigned long long>();

	for (auto numStr = std::string(); std::getline(input, numStr, ','); )
	{
		nums.push_back(std::stoi(numStr));
	}

	auto result = game.RunGame(nums, 30000000);
	std::cout << "Final result after 30000000 runs is: " << result << std::endl;
}

