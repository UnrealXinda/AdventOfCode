#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <optional>
#include <numeric>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include "Day20.h"

namespace
{
	struct Tile
	{
		Tile();
		Tile(int idx, const std::vector<std::string>& strs);
		void Transform();

		int index;
		int rotationCount;
		bool flipped;
		std::string top;
		std::string bottom;
		std::string left;
		std::string right;
	};

	struct Coordinate
	{
		Coordinate(int _x, int _y): x(_x), y(_y) {}
		int x;
		int y;
	};

	struct Grid
	{
		Grid(unsigned int _row, unsigned int _col);
		auto operator[](const Coordinate& c) -> Tile&;
		auto operator[](const Coordinate& c) const -> const Tile&;

		auto GetNextAvailableSlot() const -> std::optional<Coordinate>;
		auto CanFit(const Coordinate& c, const Tile& tile) const -> bool;
		auto ToStrings(const std::unordered_map<int, std::vector<std::string>>& patternMap) const -> std::vector<std::string>;

		unsigned int row;
		unsigned int col;
		std::vector<Tile> slots;
	};

	Tile::Tile() : index(-1), top(), bottom(), left(), right(), flipped(false), rotationCount(0)
	{
	}

	Tile::Tile(int idx, const std::vector<std::string>& strs) : index(idx), top(), bottom(), left(), right(), flipped(false), rotationCount(0)
	{
		assert(std::all_of(strs.cbegin(), strs.cend(), [&strs](const auto& str) { return str.length() == strs.size(); }));

		auto leftStream = std::stringstream();
		auto rightStream = std::stringstream();
		top = strs[0];
		bottom = strs[strs.size() - 1];

		for (const auto& str : strs)
		{
			leftStream << str[0];
			rightStream << str[str.length() - 1];
		}

		left = leftStream.str();
		right = rightStream.str();
	}	

	Grid::Grid(unsigned int _row, unsigned int _col) :
		row(_row), col(_col),
		slots(_row * _col)
	{
	}

	auto Grid::operator[](const Coordinate& c) -> Tile&
	{
		assert(c.x >= 0 && c.x < col && c.y >= 0 && c.y < row);
		return slots[c.y * col + c.x];
	}

	auto Grid::operator[](const Coordinate& c) const -> const Tile&
	{
		assert(c.x >= 0 && c.x < col && c.y >= 0 && c.y < row);
		return slots[c.y * col + c.x];
	}

	auto Grid::GetNextAvailableSlot() const -> std::optional<Coordinate>
	{
		auto it = std::find_if(std::cbegin(slots), std::cend(slots), [](const auto& t) { return t.index < 0; });
		if (it != std::cend(slots))
		{
			auto idx = std::distance(std::cbegin(slots), it);
			auto x = idx % col;
			auto y = idx / col;
			return std::make_optional<Coordinate>(x, y);
		}

		return std::nullopt;
	}

	auto Grid::CanFit(const Coordinate& c, const Tile& tile) const -> bool
	{
		assert(c.x >= 0 && c.x < col && c.y >= 0 && c.y < row);
		auto canFit = true;
		canFit &= (c.x == 0) || (tile.left == (*this)[{ c.x - 1, c.y }].right);
		canFit &= (c.y == 0) || (tile.top == (*this)[{ c.x, c.y - 1 }].bottom);
		return canFit;
	}

	auto Grid::ToStrings(const std::unordered_map<int, std::vector<std::string>>& patternMap) const -> std::vector<std::string>
	{
		auto patternSize = slots[0].left.length();
		auto streams = std::vector<std::stringstream>(patternSize - 2);
		auto strings = std::vector<std::string>();

		for (auto y = 0; y < row; ++y)
		{
			for (auto x = 0; x < col; ++x)
			{
				const auto& tile = (*this)[{x, y}];
				const auto& patterns = patternMap.at(tile.index);

				for (auto i = 1; i < patternSize - 1; ++i)
				{
					for (auto j = 1; j < patterns[i].length() - 1; ++j)
					{
						streams[i - 1] << patterns[i][j];
					}
				}
			}

			for (auto& stream : streams)
			{
				strings.push_back(std::move(stream.str()));
				stream.clear();
			}
		}

		return strings;
	}

	auto RotateClockwise(const Tile& orig) -> Tile
	{
		auto tile = orig;
		auto temp = std::move(tile.top);
		tile.top = std::move(tile.left);
		tile.left = std::move(tile.bottom);
		tile.bottom = std::move(tile.right);
		tile.right = std::move(temp);

		std::reverse(tile.top.begin(), tile.top.end());
		std::reverse(tile.bottom.begin(), tile.bottom.end());

		++tile.rotationCount;

		return tile;
	}

	auto FlipHorizontally(const Tile& orig) -> Tile
	{
		auto tile = orig;
		std::swap(tile.left, tile.right);
		tile.flipped = !tile.flipped;

		std::reverse(tile.top.begin(), tile.top.end());
		std::reverse(tile.bottom.begin(), tile.bottom.end());
		return tile;
	}

	auto GetPermutations(const Tile& orig) -> std::vector<Tile>
	{
		auto permutations = std::vector<Tile>(8);
		permutations[0] = orig;

		for (auto i = 1; i <= 3; ++i)
		{
			permutations[i] = std::move(RotateClockwise(permutations[i - 1]));
		}

		permutations[4] = FlipHorizontally(orig);

		for (auto i = 5; i <= 7; ++i)
		{
			permutations[i] = std::move(RotateClockwise(permutations[i - 1]));
		}

		return permutations;
	}

	auto DfsSolver(const Grid& grid, const std::vector<Tile>& tiles) -> std::optional<Grid>
	{
		if (tiles.size() == 0)
		{
			return std::make_optional<Grid>(grid);
		}

		auto slot = grid.GetNextAvailableSlot();

		if (!slot)
		{
			return std::nullopt;
		}

		for (const auto& tile : tiles)
		{
			auto remainingTiles = tiles;
			remainingTiles.erase(std::remove_if(remainingTiles.begin(),
			                                    remainingTiles.end(),
												[&tile](const auto& t)
												{
													return t.index == tile.index;
												}),
								remainingTiles.end());

			auto permutations = GetPermutations(tile);

			for (auto& permutation : permutations)
			{
				if (grid.CanFit(*slot, permutation))
				{
					auto newGrid = grid;
					newGrid[*slot] = std::move(permutation);

					auto validGrid = DfsSolver(newGrid, remainingTiles);

					if (validGrid)
					{
						return validGrid;
					}
				}
			}
		}

		return std::nullopt;
	}

	void Rotate2dArrayClockWise(std::vector<std::string>& strs)
	{
		auto size = strs.size();

		for (auto x = 0; x < size / 2; ++x)
		{
			for (int y = x; y < size - x - 1; ++y)
			{
				auto temp = strs[x][y];
				strs[x][y] = strs[size - 1 - y][x];
				strs[size - 1 - y][x] = strs[size - 1 - x][size - 1 - y];
				strs[size - 1 - x][size - 1 - y] = strs[y][size - 1 - x];
				strs[y][size - 1 - x] = temp;
			}
		}
	}

	void Flip2dArrayDiagonally(std::vector<std::string>& strs)
	{
		auto size = strs.size();

		for (auto y = 0; y < size; ++y)
		{
			for (int x = 0; x <= y; ++x)
			{
				std::swap(strs[x][y], strs[y][x]);
			}
		}
	}

	void TransformTile(std::unordered_map<int, std::vector<std::string>>& patternMap, Tile& tile)
	{
		if (patternMap.find(tile.index) != patternMap.end())
		{
			auto& patterns = patternMap[tile.index];

			if (tile.flipped)
			{
				std::for_each(patterns.begin(), patterns.end(), [](auto& pattern)
				{
					std::reverse(pattern.begin(), pattern.end());
				});
			}

			for (auto i = 0; i < tile.rotationCount % 4; ++i)
			{
				Rotate2dArrayClockWise(patterns);
			}

			tile.flipped = false;
			tile.rotationCount = 0;
		}
	}

	auto GetMatchIndices(const std::string& str, const std::regex& regex) -> std::vector<int>
	{
		auto match = std::smatch();
		auto indices = std::vector<int>();
		auto startIdx = 0;

		while (std::regex_search(std::next(str.cbegin(), startIdx), str.cend(), match, regex))
		{
			indices.push_back(startIdx + match.position());
			startIdx += match.position() + 1;
		}

		return indices;
	}

	auto GetMatchIndicesFromStrings(const std::vector<std::string>& strs, const std::vector<std::regex>& regexes) -> std::unordered_map<int, std::vector<int>>
	{
		auto validIndices = std::unordered_map<int, std::vector<int>>();

		for (auto i = 0; i < strs.size() - regexes.size(); ++i)
		{
			auto indices = GetMatchIndices(strs[i], regexes[0]);

			for (auto j = 1; j < regexes.size(); ++j)
			{
				auto temp = std::vector<int>();
				auto newIndices = GetMatchIndices(strs[i + j], regexes[j]);
				std::set_intersection(std::cbegin(newIndices), std::cend(newIndices), std::cbegin(indices), std::cend(indices), std::back_inserter(temp));
				indices.swap(temp);
			}

			if (indices.size() > 0)
			{
				validIndices[i] = std::move(indices);
			}
		}

		return validIndices;
	}
}

void Day20::SolutionPartOne(const std::string& inputFilePath)
{
	const auto titleRegex = std::regex(R"rgx(^Tile (\d+):$)rgx");
	const auto tileRegex = std::regex(R"rgx(^([.#]+)$)rgx");
	auto input = std::ifstream(inputFilePath);
	auto tiles = std::vector<Tile>();
	auto strs = std::vector<std::string>();
	auto match = std::smatch();
	auto tileIdx = 0;

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, titleRegex))
		{
			tileIdx = std::stoi(match[1].str());
		}
		else if (std::regex_match(line, match, tileRegex))
		{
			strs.push_back(std::move(line));
		}
		else
		{
			tiles.emplace_back(tileIdx, strs);
			strs.clear();
		}
	}
	tiles.emplace_back(tileIdx, strs);

	auto row = static_cast<int>(std::sqrt(tiles.size()));
	assert(std::pow(row, 2) == tiles.size());

	auto finalGrid = DfsSolver(Grid(row, row), tiles);

	if (finalGrid)
	{
		auto topLeftIdx = static_cast<unsigned long long>((*finalGrid)[{0, 0}].index);
		auto topRightIdx = static_cast<unsigned long long>((*finalGrid)[{row - 1, 0}].index);
		auto bottomLeftIdx = static_cast<unsigned long long>((*finalGrid)[{0, row - 1}].index);
		auto bottomRightIdx = static_cast<unsigned long long>((*finalGrid)[{row - 1, row - 1}].index);
		auto product = topLeftIdx * topRightIdx * bottomLeftIdx * bottomRightIdx;

		std::cout << "Top left: " << topLeftIdx << std::endl;
		std::cout << "Top right: " << topRightIdx << std::endl;
		std::cout << "Bottom left: " << bottomLeftIdx << std::endl;
		std::cout << "Bottom right: " << bottomRightIdx << std::endl;
		std::cout << "Total product: " << product << std::endl;
	}
}

void Day20::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto monsterPatternCount = 3;
	const auto monsterPoundCount = 15;
	const auto monsterPatterns = std::array<std::string, monsterPatternCount>
	{
		"..................#.",
		"#....##....##....###",
		".#..#..#..#..#..#..."
	};
	const auto titleRegex = std::regex(R"rgx(^Tile (\d+):$)rgx");
	const auto tileRegex = std::regex(R"rgx(^([.#]+)$)rgx");
	auto input = std::ifstream(inputFilePath);
	auto patternMap = std::unordered_map<int, std::vector<std::string>>();
	auto tiles = std::vector<Tile>();
	auto strs = std::vector<std::string>();
	auto match = std::smatch();
	auto tileIdx = 0;

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, titleRegex))
		{
			tileIdx = std::stoi(match[1].str());
		}
		else if (std::regex_match(line, match, tileRegex))
		{
			strs.push_back(std::move(line));
		}
		else
		{
			tiles.emplace_back(tileIdx, strs);
			patternMap[tileIdx] = std::move(strs);
			strs.clear();
		}
	}
	tiles.emplace_back(tileIdx, strs);
	patternMap[tileIdx] = std::move(strs);

	auto row = static_cast<int>(std::sqrt(tiles.size()));
	assert(std::pow(row, 2) == tiles.size());

	auto finalGrid = DfsSolver(Grid(row, row), tiles);

	if (finalGrid)
	{
		for (auto& tile : finalGrid->slots)
		{
			TransformTile(patternMap, tile);
		}

		auto patterns = finalGrid->ToStrings(patternMap);
		auto monsterRegexes = std::vector<std::regex>();
		std::transform(std::cbegin(monsterPatterns), std::cend(monsterPatterns), std::back_inserter(monsterRegexes), [](const auto& str)
		{
			return std::regex(str);
		});

		auto validIndices = GetMatchIndicesFromStrings(patterns, monsterRegexes);

		if (validIndices.size() == 0)
		{
			Flip2dArrayDiagonally(patterns);
			validIndices = GetMatchIndicesFromStrings(patterns, monsterRegexes);
		}

		auto poundSignCount = std::accumulate(patterns.cbegin(), patterns.cend(), 0, [](auto count, const auto& str)
		{
			return count + std::count(str.cbegin(), str.cend(), '#');
		});

		auto totalMonsterCount = std::accumulate(validIndices.cbegin(), validIndices.cend(), 0, [](auto count, const auto& pair)
		{
			return count + pair.second.size();
		});

		std::cout << poundSignCount - totalMonsterCount * monsterPoundCount << std::endl;
	}

}