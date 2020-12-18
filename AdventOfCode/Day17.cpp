#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <assert.h>
#include "Day17.h"

namespace
{
	enum class Cube
	{
		Inactive = 0,
		Active = 1,
	};

	struct Coordinate
	{
		int x;
		int y;
		int z;

		auto operator+(const Coordinate& c) -> Coordinate;
	};

	auto Coordinate::operator+(const Coordinate& c) -> Coordinate
	{
		auto coord = Coordinate();
		coord.x = x + c.x;
		coord.y = y + c.y;
		coord.z = z + c.z;
		return coord;
	}

	struct HyperCoordinate
	{
		int x;
		int y;
		int z;
		int w;

		auto operator+(const HyperCoordinate& c) -> HyperCoordinate;
	};

	auto HyperCoordinate::operator+(const HyperCoordinate& c) -> HyperCoordinate
	{
		auto coord = HyperCoordinate();
		coord.x = x + c.x;
		coord.y = y + c.y;
		coord.z = z + c.z;
		coord.w = w + c.w;
		return coord;
	}

	struct Space
	{
		Space(int l, int w, int h) : length(l), width(w), height(h), cubes(l * w * h) {}
		auto GetActiveCount() const->std::size_t;

		auto operator[](const Coordinate& c) -> Cube&;
		auto operator[](const Coordinate& c) const -> const Cube&;

		int length;
		int width;
		int height;
		std::vector<Cube> cubes;
	};

	auto Space::GetActiveCount() const -> std::size_t
	{
		return std::count(std::cbegin(cubes), std::cend(cubes), Cube::Active);
	}

	auto Space::operator[](const Coordinate& c) -> Cube&
	{
		assert(c.x >= 0 && c.x < length);
		assert(c.y >= 0 && c.y < width);
		assert(c.z >= 0 && c.z < height);
		return cubes[c.z * width * length + c.y * length + c.x];
	}

	auto Space::operator[](const Coordinate& c) const -> const Cube&
	{
		assert(c.x >= 0 && c.x < length);
		assert(c.y >= 0 && c.y < width);
		assert(c.z >= 0 && c.z < height);
		return cubes[c.z * width * length + c.y * length + c.x];
	}

	struct HyperSpace
	{
		HyperSpace(int l, int w, int h, int t) : length(l), width(w), height(h), time(t), cubes(l * w * h * t) {}
		auto GetActiveCount() const->std::size_t;

		auto operator[](const HyperCoordinate& c) -> Cube&;
		auto operator[](const HyperCoordinate& c) const -> const Cube&;

		int length;
		int width;
		int height;
		int time;
		std::vector<Cube> cubes;

		void Print() const
		{
			for (auto w = 0; w < time; ++w)
			{
				for (auto z = 0; z < height; ++z)
				{
					std::cout << "W = " << w << ", Z = " << z << std::endl;
					for (auto y = 0; y < width; ++y)
					{
						for (auto x = 0; x < length; ++x)
						{
							std::cout << (cubes[w * height * width * length + z * width * length + y * length + x] == Cube::Active ? '#' : '.');
						}
						std::cout << std::endl;
					}
				}
			}
		}
	};

	auto HyperSpace::GetActiveCount() const->std::size_t
	{
		return std::count(std::cbegin(cubes), std::cend(cubes), Cube::Active);
	}

	auto HyperSpace::operator[](const HyperCoordinate& c) -> Cube&
	{
		assert(c.x >= 0 && c.x < length);
		assert(c.y >= 0 && c.y < width);
		assert(c.z >= 0 && c.z < height);
		assert(c.w >= 0 && c.w < time);
		return cubes[c.w * height * width * length + c.z * width * length + c.y * length + c.x];
	}

	auto HyperSpace::operator[](const HyperCoordinate& c) const -> const Cube&
	{
		assert(c.x >= 0 && c.x < length);
		assert(c.y >= 0 && c.y < width);
		assert(c.z >= 0 && c.z < height);
		assert(c.w >= 0 && c.w < time);
		return cubes[c.w * height * width * length + c.z * width * length + c.y * length + c.x];
	}

	inline auto ExamineSpace(const Space& space, Coordinate c) -> Cube
	{
		if (c.x >= 0 && c.x < space.length &&
			c.y >= 0 && c.y < space.width &&
			c.z >= 0 && c.z < space.height)
		{
			return space[c];
		}

		return Cube::Inactive;
	}

	inline auto ExamineHyperSpace(const HyperSpace& space, HyperCoordinate c) -> Cube
	{
		if (c.x >= 0 && c.x < space.length &&
			c.y >= 0 && c.y < space.width &&
			c.z >= 0 && c.z < space.height &&
			c.w >= 0 && c.w < space.time)
		{
			return space[c];
		}

		return Cube::Inactive;
	}

	auto ApplyFilter(const Space& space, Coordinate c) -> Cube
	{
		static const auto neighboors = std::array<Coordinate, 26>
		{{
			{ -1, -1, -1 }, { -1, -1,  0 }, { -1, -1,  1 }, { -1,  0, -1 }, { -1,  0,  0 }, { -1,  0,  1 }, { -1,  1, -1 }, { -1,  1,  0 }, { -1,  1,  1 },
			{  0, -1, -1 }, {  0, -1,  0 }, {  0, -1,  1 }, {  0,  0, -1 },                 {  0,  0,  1 }, {  0,  1, -1 }, {  0,  1,  0 }, {  0,  1,  1 },
			{  1, -1, -1 }, {  1, -1,  0 }, {  1, -1,  1 }, {  1,  0, -1 }, {  1,  0,  0 }, {  1,  0,  1 }, {  1,  1, -1 }, {  1,  1,  0 }, {  1,  1,  1 },
		}};

		auto activeCount = std::count_if(std::cbegin(neighboors), std::cend(neighboors), [&](const auto& neighboor)
		{
			return ExamineSpace(space, c + neighboor) == Cube::Active;
		});

		auto cube = space[c];
		if (cube == Cube::Active && (activeCount != 2 && activeCount != 3))
		{
			return Cube::Inactive;
		}

		if (cube == Cube::Inactive && activeCount == 3)
		{
			return Cube::Active;
		}

		return cube;
	}

	auto ApplyHyperFilter(const HyperSpace& space, HyperCoordinate c) -> Cube
	{
		auto activeCount = 0;

		for (auto w = -1; w <= 1; ++w)
		{
			for (auto z = -1; z <= 1; ++z)
			{
				for (auto y = -1; y <= 1; ++y)
				{
					for (auto x = -1; x <= 1; ++x)
					{
						if (!(x == 0 && y == 0 && z == 0 && w == 0))
						{
							activeCount += ExamineHyperSpace(space, c + HyperCoordinate{x, y, z, w}) == Cube::Active;
						}
					}
				}
			}
		}

		auto cube = space[c];
		if (cube == Cube::Active && (activeCount != 2 && activeCount != 3))
		{
			return Cube::Inactive;
		}

		if (cube == Cube::Inactive && activeCount == 3)
		{
			return Cube::Active;
		}

		return cube;
	}

	auto SuperSpacing(const Space& space) -> Space
	{
		auto newSpace = Space(space.width + 2, space.length + 2, space.height + 2);
		for (auto z = 1; z <= space.height; ++z)
		{
			for (auto y = 1; y <= space.width; ++y)
			{
				for (auto x = 1; x <= space.length; ++x)
				{
					newSpace[{x, y, z}] = space[{x - 1, y - 1, z - 1}];
				}
			}
		}
		return newSpace;
	}
	
	auto SuperHyperSpacing(const HyperSpace& space) -> HyperSpace
	{
		auto newSpace = HyperSpace(space.width + 2, space.length + 2, space.height + 2, space.time + 2);
		for (auto w = 1; w <= space.time; ++w)
		{
			for (auto z = 1; z <= space.height; ++z)
			{
				for (auto y = 1; y <= space.width; ++y)
				{
					for (auto x = 1; x <= space.length; ++x)
					{
						newSpace[{x, y, z, w}] = space[{x - 1, y - 1, z - 1, w - 1}];
					}
				}
			}
		}
		return newSpace;
	}

	auto ThreeDimensionalConvolution(const Space& space) -> Space
	{
		auto result = Space(space.width + 2, space.length + 2, space.height + 2);
		auto superSpace = SuperSpacing(space);

		for (auto z = 0; z < superSpace.height; ++z)
		{
			for (auto y = 0; y < superSpace.width; ++y)
			{
				for (auto x = 0; x < superSpace.length; ++x)
				{
					result[{x, y, z}] = ApplyFilter(superSpace, { x, y, z });
				}
			}
		}

		return result;
	}

	auto FourDimensionalConvolution(const HyperSpace& space) -> HyperSpace
	{
		auto result = HyperSpace(space.width + 2, space.length + 2, space.height + 2, space.time + 2);
		auto superSpace = SuperHyperSpacing(space);

		for (auto w = 0; w < superSpace.time; ++w)
		{
			for (auto z = 0; z < superSpace.height; ++z)
			{
				for (auto y = 0; y < superSpace.width; ++y)
				{
					for (auto x = 0; x < superSpace.length; ++x)
					{
						result[{x, y, z, w}] = ApplyHyperFilter(superSpace, { x, y, z, w });
					}
				}
			}
		}

		return result;
	}
}

void Day17::SolutionPartOne(const std::string& inputFilePath)
{
	const auto kCycles = 6;
	auto input = std::ifstream(inputFilePath);
	auto lines = std::vector<std::string>();

	for (std::string line; std::getline(input, line); )
	{
		lines.push_back(std::move(line));
	}

	auto space = Space(lines[0].length(), lines.size(), 1);

	for (auto j = 0; j < lines.size(); ++j)
	{
		for (auto i = 0; i < lines[j].length(); ++i)
		{
			switch (lines[j][i])
			{
			case '.':
				space[{i, j, 0}] = Cube::Inactive;
				break;
			case '#':
				space[{i, j, 0}] = Cube::Active;
				break;
			}
		}
	}

	for (auto i = 0; i < kCycles; ++i)
	{
		space = ThreeDimensionalConvolution(std::move(space));
	}

	std::cout << "Final active cube count: " << space.GetActiveCount() << std::endl;
}

void Day17::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto kCycles = 6;
	auto input = std::ifstream(inputFilePath);
	auto lines = std::vector<std::string>();

	for (std::string line; std::getline(input, line); )
	{
		lines.push_back(std::move(line));
	}

	auto space = HyperSpace(lines[0].length(), lines.size(), 1, 1);

	for (auto j = 0; j < lines.size(); ++j)
	{
		for (auto i = 0; i < lines[j].length(); ++i)
		{
			switch (lines[j][i])
			{
			case '.':
				space[{i, j, 0, 0}] = Cube::Inactive;
				break;
			case '#':
				space[{i, j, 0, 0}] = Cube::Active;
				break;
			}
		}
	}

	for (auto i = 0; i < kCycles; ++i)
	{
		space = FourDimensionalConvolution(std::move(space));
	}

	std::cout << "Final active cube count: " << space.GetActiveCount() << std::endl;
}