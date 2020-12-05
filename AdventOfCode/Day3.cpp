#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "Day3.h"

namespace
{
	struct TreeMap
	{
	public:		

		void AppendPattern(const std::string& pattern);
		auto HasTree(int x, int y) const -> bool;
		auto CountEncounteredTrees(int xStep, int yStep) const -> std::size_t;

	private:
		std::vector<std::vector<bool>> m_landscape;
		std::size_t m_patternWidth;
	};

	void TreeMap::AppendPattern(const std::string& pattern)
	{
		assert(m_patternWidth == 0 || pattern.length() == m_patternWidth);

		m_patternWidth = pattern.length();
		auto newLine = std::vector<bool>(m_patternWidth);

		std::transform(pattern.cbegin(),
		               pattern.cend(),
					   newLine.begin(),
					   [](const char ch) -> bool { return ch == '#'; });

		m_landscape.push_back(std::move(newLine));
	}

	auto TreeMap::HasTree(int x, int y) const -> bool
	{
		assert(x >= 0 && x < m_patternWidth);
		assert(y >= 0 && y < m_landscape.size());

		return m_landscape[y][x];
	}

	auto TreeMap::CountEncounteredTrees(int xStep, int yStep) const -> std::size_t
	{
		auto count = 0;

		for (auto x = 0, y = 0; y < m_landscape.size(); x += xStep, y += yStep)
		{
			count += HasTree(x % m_patternWidth, y);
		}

		return count;
	}
}

void Day3::SolutionPartOne(const std::string& inputFilePath)
{
	const auto kDeltaX = 3;
	const auto kTree = '#';
	auto x = 0, count = 0;
	auto input = std::ifstream(inputFilePath);

	for (std::string line; std::getline(input, line); )
	{
		count += line[x % line.length()] == kTree;
		x += kDeltaX;
	}

	std::cout << "Total trees encountered: " << count << std::endl;
}

void Day3::SolutionPartTwo(const std::string& inputFilePath)
{
	const std::pair<int, int> kSteps[] = {{1, 1},{3, 1},{5, 1},{7, 1},{1, 2}};

	auto product = std::size_t(1);
	auto input = std::ifstream(inputFilePath);
	auto treeMap = TreeMap();

	for (std::string line; std::getline(input, line); )
	{
		treeMap.AppendPattern(line);
	}

	std::for_each(std::cbegin(kSteps), std::cend(kSteps), [&](const auto& pair) mutable
	{
		product *= treeMap.CountEncounteredTrees(std::get<0>(pair), std::get<1>(pair));
	});

	std::cout << "Total trees encountered: " << product << std::endl;
}

