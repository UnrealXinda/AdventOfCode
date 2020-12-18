//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <array>
//#include <algorithm>
//#include <assert.h>
//#include "Day17.h"
//
//namespace
//{
//	enum class Cube
//	{
//		Inactive = 0,
//		Active = 1,
//	};
//
//	//struct Coordinate
//	//{
//	//	int x;
//	//	int y;
//	//	int z;
//
//	//	auto operator+(const Coordinate& c) -> Coordinate;
//	//};
//
//	//auto Coordinate::operator+(const Coordinate& c) -> Coordinate
//	//{
//	//	auto coord = Coordinate();
//	//	coord.x = x + c.x;
//	//	coord.y = y + c.y;
//	//	coord.z = z + c.z;
//	//	return coord;
//	//}
//
//	//struct HyperCoordinate
//	//{
//	//	int x;
//	//	int y;
//	//	int z;
//	//	int w;
//
//	//	auto operator+(const HyperCoordinate& c) -> HyperCoordinate;
//	//};
//
//	//auto HyperCoordinate::operator+(const HyperCoordinate& c) -> HyperCoordinate
//	//{
//	//	auto coord = HyperCoordinate();
//	//	coord.x = x + c.x;
//	//	coord.y = y + c.y;
//	//	coord.z = z + c.z;
//	//	coord.w = w + c.w;
//	//	return coord;
//	//}
//
//	template <unsigned int Dim, class T>
//	struct Coordinate
//	{
//		std::array<T, Dim> coords;
//
//		template <unsigned int Dim, class T>
//		auto operator[](T idx)->T&;
//
//		template <unsigned int Dim, class T>
//		auto operator[](T idx) const -> const T&;
//
//		template <unsigned int Dim, class T>
//		auto operator+(const Coordinate<Dim, T>& c)->Coordinate<Dim, T>;
//	};
//
//	template <unsigned int Dim, class T>
//	auto Coordinate::operator[](T idx) -> T&
//	{
//		return coords[idx];
//	}
//
//	template <unsigned int Dim, class T>
//	auto Coordinate::operator[](T idx) const -> const T&
//	{
//		return coords[idx];
//	}
//
//	template <unsigned int Dim, class T>
//	auto Coordinate::operator+(const Coordinate<Dim, T>& c) -> Coordinate<Dim, T>
//	{
//		auto coord = Coordinate<Dim, T>();
//		std::transform(std::cbegin(coords), std::cend(coords), std::cbegin(c.coords), std::begin(coord.coords), std::plus<T>());
//		return coord;
//	}
//
//	template <unsigned int Dim, class T>
//	struct Space
//	{
//		Space(std::array<T, Dim> inputs) : dims(inputs) {}
//		auto GetActiveCount() const->std::size_t;
//
//		template <unsigned int Dim, class T>
//		auto operator[](const Coordinate<Dim, T>& c)->Cube&;
//
//		template <unsigned int Dim, class T>
//		auto operator[](const Coordinate<Dim, T>& c) const -> const Cube&;
//
//		std::array<T, Dim> dims;
//		std::vector<Cube> cubes;
//	};
//
//	template <unsigned int Dim, class T>
//	auto Space::GetActiveCount() const -> std::size_t
//	{
//		return std::count(std::cbegin(cubes), std::cend(cubes), Cube::Active);
//	}
//
//	template <unsigned int Dim, class T>
//	auto Space::operator[](const Coordinate<Dim, T>& c) -> Cube&
//	{
//		auto index = 0;
//		auto stride = 1;
//		for (auto i = 0; i < Dim; ++i)
//		{
//			if (c[i] >= 0 && c[i] < dims[i]))
//			{
//			index += stride * c[i];
//			stride *= dims[i - 1];
//			}
//			else
//			{
//				return Cube::Inactive;
//			}
//		}
//		return cubes[index];
//	}
//
//	template <unsigned int Dim, class T>
//	auto Space::operator[](const Coordinate<Dim, T>& c) const -> const Cube&
//	{
//		auto index = 0;
//		auto stride = 1;
//		for (auto i = 0; i < Dim; ++i)
//		{
//			if (c[i] >= 0 && c[i] < dims[i]))
//			{
//			index += stride * c[i];
//			stride *= dims[i - 1];
//			}
//			else
//			{
//				return Cube::Inactive;
//			}
//		}
//		return cubes[index];
//	}
//
//	template <unsigned int Dim, class T>
//	inline auto ExamineSpace(const Space<Dim, T>& space, Coordinate<Dim, T> c) -> Cube
//	{
//		for (auto i = 0; i < Dim; ++i)
//		{
//			if (c[i] < 0 || c[i] >= space.dims[i])
//			{
//				return Cube::Inactive;
//			}
//		}
//		return space[c];
//	}
//
//	template <unsigned int Dim, class T>
//	auto ApplyFilter(const Space<Dim, T>& space, Coordinate<Dim, T> c) -> Cube
//	{
//		auto activeNeighboorCount = 0;
//		auto neighboors = std::vector<T>();
//		neighboors.insert(neighboors.end(), Dim, -1);
//		neighboors.insert(neighboors.end(), Dim, 0);
//		neighboors.insert(neighboors.end(), Dim, 1);
//
//		do
//		{
//			auto neighboor = Coordinate<Dim, T>();
//			for (auto i = 0; i < Dim; ++i)
//			{
//				neighboor.coords[i] = neighboors[i];
//			}
//
//			activeNeighboorCount += std::any_of(std::cbegin(neighboor.coords), std::cend(neighboor.coords), [](auto i) { return i != 0; })
//				&& ExamineSpace(space, c + neighboor) == Cube::Active;
//		} while (next_permutation(neighboors.begin(), neighboors.end()));
//
//		if (space[c] == Cube::Active && (activeNeighboorCount != 2 && activeNeighboorCount != 3))
//		{
//			return Cube::Inactive;
//		}
//
//		if (space[c] == Cube::Inactive && activeNeighboorCount == 3)
//		{
//			return Cube::Active;
//		}
//
//		return space[c];
//	}
//
//	template <unsigned int Dim, class T>
//	auto SuperSpacing(const Space& space) -> Space
//	{
//		auto newDim = space.dims;
//		std::transform(std::begin(newDim), std::end(newDim), std::begin(newDim), [](auto i) { return i + 2; });
//
//		auto newSpace = Space(newDim);
//		auto idx = 0;
//		std::for_each(newSpace.cubes.begin(), newSpace.cubes.end())
//
//
//			for (auto z = 1; z <= space.height; ++z)
//			{
//				for (auto y = 1; y <= space.width; ++y)
//				{
//					for (auto x = 1; x <= space.length; ++x)
//					{
//						newSpace[{x, y, z}] = space[{x - 1, y - 1, z - 1}];
//					}
//				}
//			}
//		return newSpace;
//	}
//
//	auto ThreeDimensionalConvolution(const Space& space) -> Space
//	{
//		auto result = Space(space.width + 2, space.length + 2, space.height + 2);
//		auto superSpace = SuperSpacing(space);
//
//		for (auto z = 0; z < superSpace.height; ++z)
//		{
//			for (auto y = 0; y < superSpace.width; ++y)
//			{
//				for (auto x = 0; x < superSpace.length; ++x)
//				{
//					result[{x, y, z}] = ApplyFilter(superSpace, { x, y, z });
//				}
//			}
//		}
//
//		return result;
//	}
//}
//
//void Day17::SolutionPartOne(const std::string& inputFilePath)
//{
//	const auto kCycles = 6;
//	auto input = std::ifstream(inputFilePath);
//	auto lines = std::vector<std::string>();
//
//	for (std::string line; std::getline(input, line); )
//	{
//		lines.push_back(std::move(line));
//	}
//
//	auto space = Space(lines[0].length(), lines.size(), 1);
//
//	for (auto j = 0; j < lines.size(); ++j)
//	{
//		for (auto i = 0; i < lines[j].length(); ++i)
//		{
//			switch (lines[j][i])
//			{
//			case '.':
//				space[{i, j, 0}] = Cube::Inactive;
//				break;
//			case '#':
//				space[{i, j, 0}] = Cube::Active;
//				break;
//			}
//		}
//	}
//
//	for (auto i = 0; i < kCycles; ++i)
//	{
//		space = ThreeDimensionalConvolution(std::move(space));
//	}
//
//	std::cout << "Final active cube count: " << space.GetActiveCount() << std::endl;
//}
//
//void Day17::SolutionPartTwo(const std::string& inputFilePath)
//{
//}
//
