#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Day23.h"

namespace
{
	using Cup = unsigned int;

	struct Cups
	{
		explicit Cups(const std::string& str);
		std::vector<Cup> cups;

		void RotateLeft(unsigned int n);
		void RotateRight(unsigned int n);
	};

	struct MassiveCups
	{
		std::unordered_map<Cup, Cup> cups;
	};

	Cups::Cups(const std::string& str)
	{
		for (auto i = 0; i < str.length(); ++i)
		{
			cups.emplace_back(str[i] - '0');
		}
	}

	void Cups::RotateLeft(unsigned int n)
	{
		auto size = cups.size();
		n = n % size;
		auto begin = std::begin(cups);
		auto end = std::end(cups);
		std::rotate(begin, std::next(begin, n), end);
	}

	void Cups::RotateRight(unsigned int n)
	{
		auto size = cups.size();
		n = size - n % size;
		RotateLeft(n);
	}

	class CupGame
	{
	public:
		void RunGame(Cups& cups, unsigned int neighborCount, unsigned int round);
	private:
		void RunGameOneRound(Cups& cups, unsigned int neighborCount);
	};

	void CupGame::RunGame(Cups& cups, unsigned int neighborCount, unsigned int round)
	{
		for (auto i = 0; i < round; ++i)
		{
			RunGameOneRound(cups, neighborCount);
			cups.RotateLeft(1);
		}

		cups.RotateRight(round);
	}

	void CupGame::RunGameOneRound(Cups& cups, unsigned int neighborCount)
	{
		auto size = cups.cups.size();
		auto value = (cups.cups.front() + size - 2) % size + 1;
		
		auto neighborBegin = std::next(std::begin(cups.cups), 1);
		auto neighborEnd = std::next(neighborBegin, neighborCount);
		while (std::find(neighborBegin, neighborEnd, value) != neighborEnd)
		{
			value = (value + size - 2) % size + 1;
		}

		auto it = std::find(neighborEnd, std::end(cups.cups), value);
		std::rotate(neighborBegin, neighborEnd, std::next(it, 1));
	}

	class MassiveCupGame
	{
	public:
		void RunGame(MassiveCups& cups, unsigned int startLabel, unsigned int neighborCount, unsigned int round);
	private:
		auto RunGameOneRound(MassiveCups& cups, unsigned int startLabel, unsigned int neighborCount) -> unsigned int;
	};

	void MassiveCupGame::RunGame(MassiveCups& cups, unsigned int startLabel, unsigned int neighborCount, unsigned int round)
	{
		auto newStartLabel = startLabel;
		for (auto i = 0; i < round; ++i)
		{
			newStartLabel = RunGameOneRound(cups, newStartLabel, neighborCount);
		}
	}

	auto MassiveCupGame::RunGameOneRound(MassiveCups& cups, unsigned int startLabel, unsigned int neighborCount) -> unsigned int
	{
		auto neighbors = std::vector<Cup>(neighborCount);
		auto temp = startLabel;

		for (auto i = 0; i < neighborCount; ++i)
		{
			neighbors[i] = cups.cups[temp];
			temp = neighbors[i];
		}

		cups.cups[startLabel] = cups.cups[neighbors.back()];

		auto size = cups.cups.size();
		auto value = (startLabel + size - 2) % size + 1;
		while (std::find(std::begin(neighbors), std::end(neighbors), value) != std::end(neighbors))
		{
			value = (value + size - 2) % size + 1;
		}

		cups.cups[neighbors.back()] = cups.cups[value];
		cups.cups[value] = neighbors.front();
		return cups.cups[startLabel];
	}
}

void Day23::SolutionPartOne()
{
	constexpr auto kNeighborCount = 3;
	constexpr auto kRounds = 100;
	constexpr auto kInput = "219347865";

	auto cups = Cups(kInput);
	auto game = CupGame();
	game.RunGame(cups, kNeighborCount, kRounds);

	auto oneIt = std::find(std::begin(cups.cups), std::end(cups.cups), 1);
	std::rotate(std::begin(cups.cups), oneIt, std::end(cups.cups));

	auto num = 0LL;
	for (auto it = std::next(std::begin(cups.cups), 1); it != std::end(cups.cups); ++it)
	{
		num *= 10;
		num += *it;
	}

	std::cout << num << std::endl;
}

void Day23::SolutionPartTwo()
{
	constexpr auto kNeighborCount = 3;
	constexpr auto kRounds = 10000000;
	constexpr auto kCupsCount = 1000000;
	constexpr auto kInput = "219347865";
	constexpr auto kInputSize = 9;

	auto cups = MassiveCups();
	auto game = MassiveCupGame();

	for (auto i = 0; i < kInputSize - 1; ++i)
	{
		cups.cups[kInput[i] - '0'] = kInput[i + 1] - '0';
	}

	cups.cups[kInput[kInputSize - 1] - '0'] = kInputSize + 1;

	for (auto i = kInputSize + 1; i < kCupsCount; ++i)
	{
		cups.cups[i] = i + 1;
	}

	cups.cups[kCupsCount] = kInput[0] - '0';

	game.RunGame(cups, kInput[0] - '0', kNeighborCount, kRounds);

	auto neighborOne = static_cast<unsigned long long>(cups.cups[1]);
	auto neighborTwo = static_cast<unsigned long long>(cups.cups[neighborOne]);

	std::cout << "Neighbor One: " << neighborOne << std::endl;
	std::cout << "Neighbor Tow: " << neighborTwo << std::endl;
	std::cout << "Product: " << neighborOne * neighborTwo << std::endl;
}

