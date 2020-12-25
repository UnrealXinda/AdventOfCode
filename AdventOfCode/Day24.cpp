#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <algorithm>
#include "Day24.h"

namespace
{
	struct Tile
	{
		int x;
		int y;
	};

	enum class Color
	{
		White,
		Black
	};

	inline auto FlipColor(Color color) -> Color
	{
		return color == Color::White ? Color::Black : Color::White;
	}

	auto GetNextTileCoordinate(unsigned int& cursor, const std::string& str, int tx, int ty) -> std::tuple<int, int>
	{
		auto x = 0;
		auto y = 0;
		switch (str[cursor++])
		{
		case 's':
			y = ty - 1;
			switch (str[cursor++])
			{
			case 'w':
				x = tx - 1;
				break;
			case 'e':
				x = tx + 1;
				break;
			}
			break;
		case 'n':
			y = ty + 1;
			switch (str[cursor++])
			{
			case 'w':
				x = tx - 1;
				break;
			case 'e':
				x = tx + 1;
				break;
			}
			break;
		case 'w':
			x = tx - 2;
			y = ty;
			break;
		case 'e':
			x = tx + 2;
			y = ty;
			break;
		}

		return {x, y};
	}

	template <class Map>
	void ExecuteInstruction(const std::string& instruction, Map& map)
	{
		auto cursor = 0U;
		auto tile = Tile();

		do 
		{
			auto [x, y] = GetNextTileCoordinate(cursor, instruction, tile.x, tile.y);
			tile = Tile{x, y};
		} while (cursor < instruction.length());

		if (map.find(tile) != map.end())
		{
			map[tile] = FlipColor(map[tile]);
		}
		else
		{
			map[tile] = Color::Black;
		}
	}

	struct Floor
	{
		Floor(unsigned int w, unsigned int h);

		const unsigned int width;
		const unsigned int height;
		std::vector<Color> tiles;
	};

	Floor::Floor(unsigned int w, unsigned int h) :
		width(w),
		height(h),
		tiles(w * h)
	{
		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				tiles[y * width + x] = Color::White;
			}
		}
	}

	auto IsBlackTile(const Floor& floor, int x, int y) -> bool
	{
		if (x < 0 || x >= floor.width || y < 0 || y >= floor.height)
		{
			return false;
		}
		// Virtual tiles, should be ignored
		if (((x ^ y) & 0x01))
		{
			return false;
		}

		return floor.tiles[y * floor.width + x] == Color::Black;
	}

	auto CountAdjacentBlackTiles(const Floor& floor, unsigned int x, unsigned int y) -> std::size_t
	{
		static const auto adjacents = std::vector<std::tuple<int, int>>
		{
			{-1, -1},
			{-1,  1},
			{ 0, -2},
			{ 0,  2},
			{ 1, -1},
			{ 1,  1},
		};

		return std::count_if(adjacents.cbegin(), adjacents.cend(), [&floor, x, y](const auto& pair)
		{
			auto nx = std::get<0>(pair) + static_cast<int>(x);
			auto ny = std::get<1>(pair) + static_cast<int>(y);
			return IsBlackTile(floor, nx, ny);
		});
	}

	void FlipFloor(Floor& floor)
	{
		auto newTiles = floor.tiles;

		for (auto y = 0; y < floor.height; ++y)
		{
			for (auto x = 0; x < floor.width; ++x)
			{
				// Skip virtual tiles
				if (!((x ^ y) & 0x01))
				{
					auto count = CountAdjacentBlackTiles(floor, x, y);
					auto index = y * floor.width + x;

					if (floor.tiles[index] == Color::Black && (count == 0 || count > 2))
					{
						newTiles[index] = Color::White;
					}

					else if (floor.tiles[index] == Color::White && count == 2)
					{
						newTiles[index] = Color::Black;
					}
				}
			}
		}

		floor.tiles = std::move(newTiles);
	}
}

void Day24::SolutionPartOne(const std::string& inputFilePath)
{
	static const auto hash = [](const Tile& t) -> std::size_t
	{
		return std::hash<int>()((t.x + t.y) * (t.x + t.y + 1) / 2 + t.y); // Cantor pairing
	};
	static const auto comp = [](const Tile& l, const Tile& r) -> bool
	{
		return l.x == r.x && l.y == r.y;
	};
	auto input = std::ifstream(inputFilePath);
	auto map = std::unordered_map<Tile, Color, decltype(hash), decltype(comp)>(1000, hash, comp);
	auto startTile = Tile();

	for (auto line = std::string(); std::getline(input, line); )
	{
		ExecuteInstruction(line, map);
	}

	auto count = std::count_if(map.cbegin(), map.cend(), [](const auto& pair) { return pair.second == Color::Black; });

	std::cout << "Total flipped count: " << count << std::endl;
}

void Day24::SolutionPartTwo(const std::string& inputFilePath)
{
	constexpr auto kWidth = 200;
	constexpr auto kHeight = 200;
	constexpr auto kFlipCount = 100;
	auto input = std::ifstream(inputFilePath);
	auto floor = Floor(kWidth, kHeight);

	for (auto line = std::string(); std::getline(input, line); )
	{
		auto cursor = 0U;
		auto startX = kWidth / 2;
		auto startY = kHeight / 2;

		do
		{
			auto [x, y] = GetNextTileCoordinate(cursor, line, startX, startY);
			startX = x;
			startY = y;
		} while (cursor < line.length());

		auto tile = floor.tiles[startY * kWidth + startX];
		floor.tiles[startY * kWidth + startX] = FlipColor(tile);
	}

	for (auto i = 0; i < kFlipCount; ++i)
	{
		FlipFloor(floor);
	}

	auto count = std::count(floor.tiles.cbegin(), floor.tiles.cend(), Color::Black);
	std::cout << "Total flipped count: " << count << std::endl;
}

