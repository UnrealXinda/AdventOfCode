#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <memory>
#include <tuple>
#include <algorithm>
#include <assert.h>
#include "Day11.h"

namespace
{
	enum class Cell
	{
		Floor,
		Occupied,
		Empty,
	};

	struct SeatLayout
	{
		using Row = std::vector<Cell>;
		using Pos = std::tuple<int, int>;
		using Layout = std::vector<Row>;

		void AppendRow(const std::string& rowStr);
		void PrintLayout() const;
		auto GetOccupiedCount() const -> int;
		auto GetRowCount() const -> int;

		Row& operator[](int i);
		const Row& operator[](int i) const;		

	private:
		Layout m_layout;
	};

	class Rule
	{
	public:
		virtual auto ApplyRule(SeatLayout& newLayout, const SeatLayout& oldLayout) -> bool = 0;
	};

	class AdjacentRule final : public Rule
	{
	public:
		virtual auto ApplyRule(SeatLayout& newLayout, const SeatLayout& oldLayout) -> bool override;

	private:
		static auto ApplyRuleAtSeat(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto ApplyRuleAtFloor(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto ApplyRuleAtOccupied(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto ApplyRuleAtEmpty(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
	};

	class LinearRule final : public Rule
	{
	public:
		virtual auto ApplyRule(SeatLayout& newLayout, const SeatLayout& oldLayout) -> bool override;

	private:
		static auto ApplyRuleAtSeat(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto ApplyRuleAtFloor(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto ApplyRuleAtOccupied(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto ApplyRuleAtEmpty(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool;
		static auto IsLinearlyOccupied(const SeatLayout& layout, int x, int y, int dirX, int dirY) -> bool;
	};

	void SeatLayout::AppendRow(const std::string& rowStr)
	{
		auto row = Row();
		std::transform(rowStr.cbegin(), rowStr.cend(), std::back_inserter(row),
		[](auto ch) -> Cell
		{
			switch (ch)
			{
			case 'L':
				return Cell::Empty;
			case '#':
				return Cell::Occupied;
			case '.':
				return Cell::Floor;
			}
		});

		m_layout.push_back(std::move(row));
	}

	void SeatLayout::PrintLayout() const
	{
		for (const auto& row : m_layout)
		{
			for (const auto cell : row)
			{
				switch (cell)
				{
				case Cell::Floor:
					std::cout << ".";
					break;
				case Cell::Occupied:
					std::cout << "#";
					break;
				case Cell::Empty:
					std::cout << "L";
					break;
				}
			}
			std::cout << std::endl;
		}
	}

	auto SeatLayout::GetOccupiedCount() const -> int
	{
		return std::accumulate(m_layout.cbegin(), m_layout.cend(), 0, [](auto count, const auto& row)
		{
			return count + std::count(row.cbegin(), row.cend(), Cell::Occupied);
		});
	}

	auto SeatLayout::GetRowCount() const -> int
	{
		return m_layout.size();
	}

	SeatLayout::Row& SeatLayout::operator[](int i)
	{
		assert(i >= 0 && i < m_layout.size());
		return m_layout[i];
	}

	const SeatLayout::Row& SeatLayout::operator[](int i) const
	{
		assert(i >= 0 && i < m_layout.size());
		return m_layout[i];
	}

	auto AdjacentRule::ApplyRule(SeatLayout& newLayout, const SeatLayout& oldLayout) -> bool
	{
		auto hasChanged = false;

		// This is just 2D convolution. Wish I could just use compute shader and ping-pong buffers :-(
		for (auto y = 0; y < oldLayout.GetRowCount(); ++y)
		{
			for (auto x = 0; x < oldLayout[y].size(); ++x)
			{
				hasChanged |= ApplyRuleAtSeat(newLayout, oldLayout, x, y);
			}
		}

		return hasChanged;
	}

	auto AdjacentRule::ApplyRuleAtSeat(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		switch (oldLayout[y][x])
		{
		case Cell::Floor:
			return ApplyRuleAtFloor(newLayout, oldLayout, x, y);
		case Cell::Occupied:
			return ApplyRuleAtOccupied(newLayout, oldLayout, x, y);
		case Cell::Empty:
			return ApplyRuleAtEmpty(newLayout, oldLayout, x, y);
		default:
			return false;
		}
	}

	auto AdjacentRule::ApplyRuleAtFloor(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		assert(oldLayout[y][x] == Cell::Floor);

		newLayout[y][x] = Cell::Floor;
		return false;
	}

	auto AdjacentRule::ApplyRuleAtOccupied(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		assert(oldLayout[y][x] == Cell::Occupied);

		static const auto adjacents = std::vector<SeatLayout::Pos>
		{
			{-1, -1},
			{-1,  0},
			{-1,  1},
			{ 0, -1},
			{ 0,  1},
			{ 1, -1},
			{ 1,  0},
			{ 1,  1},
		};

		auto count = std::count_if(adjacents.cbegin(), adjacents.cend(), [&oldLayout, x, y](const auto& pos) -> bool
		{
			auto newX = x + std::get<0>(pos);
			auto newY = y + std::get<1>(pos);
			if (newY >= 0 && newY < oldLayout.GetRowCount() &&
				newX >= 0 && newX < oldLayout[y].size())
			{
				return oldLayout[newY][newX] == Cell::Occupied;
			}
			return false;
		});

		auto canBeEmpty = count >= 4;

		newLayout[y][x] = canBeEmpty ? Cell::Empty : Cell::Occupied;
		return canBeEmpty;
	}

	auto AdjacentRule::ApplyRuleAtEmpty(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		assert(oldLayout[y][x] == Cell::Empty);

		static const auto adjacents = std::vector<SeatLayout::Pos>
		{
			{-1, -1},
			{-1,  0},
			{-1,  1},
			{ 0, -1},
			{ 0,  1},
			{ 1, -1},
			{ 1,  0},
			{ 1,  1},
		};

		auto canBeOccupied = std::none_of(adjacents.cbegin(), adjacents.cend(), [&oldLayout, x, y](const auto& pos) -> bool
		{
			auto newX = x + std::get<0>(pos);
			auto newY = y + std::get<1>(pos);
			if (newY >= 0 && newY < oldLayout.GetRowCount() &&
				newX >= 0 && newX < oldLayout[y].size())
			{
				return oldLayout[newY][newX] == Cell::Occupied;
			}
			return false;
		});

		newLayout[y][x] = canBeOccupied ? Cell::Occupied : Cell::Empty;
		return canBeOccupied;
	}

	auto LinearRule::ApplyRule(SeatLayout& newLayout, const SeatLayout& oldLayout) -> bool
	{
		auto hasChanged = false;

		for (auto y = 0; y < oldLayout.GetRowCount(); ++y)
		{
			for (auto x = 0; x < oldLayout[y].size(); ++x)
			{
				hasChanged |= ApplyRuleAtSeat(newLayout, oldLayout, x, y);
			}
		}

		return hasChanged;
	}

	auto LinearRule::ApplyRuleAtSeat(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		switch (oldLayout[y][x])
		{
		case Cell::Floor:
			return ApplyRuleAtFloor(newLayout, oldLayout, x, y);
		case Cell::Occupied:
			return ApplyRuleAtOccupied(newLayout, oldLayout, x, y);
		case Cell::Empty:
			return ApplyRuleAtEmpty(newLayout, oldLayout, x, y);
		default:
			return false;
		}
	}

	auto LinearRule::ApplyRuleAtFloor(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		assert(oldLayout[y][x] == Cell::Floor);

		newLayout[y][x] = Cell::Floor;
		return false;
	}

	auto LinearRule::ApplyRuleAtOccupied(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		assert(oldLayout[y][x] == Cell::Occupied);

		static const auto dirs = std::vector<SeatLayout::Pos>
		{
			{-1, -1},
			{-1,  0},
			{-1,  1},
			{ 0, -1},
			{ 0,  1},
			{ 1, -1},
			{ 1,  0},
			{ 1,  1},
		};

		auto count = 0;

		for (const auto& dir : dirs)
		{
			count += IsLinearlyOccupied(oldLayout, x, y, std::get<0>(dir), std::get<1>(dir));
			if (count >= 5)
			{
				newLayout[y][x] = Cell::Empty;
				return true;
			}
		}

		newLayout[y][x] = Cell::Occupied;
		return false;
	}

	auto LinearRule::ApplyRuleAtEmpty(SeatLayout& newLayout, const SeatLayout& oldLayout, int x, int y) -> bool
	{
		assert(oldLayout[y][x] == Cell::Empty);

		static const auto dirs = std::vector<SeatLayout::Pos>
		{
			{-1, -1},
			{-1,  0},
			{-1,  1},
			{ 0, -1},
			{ 0,  1},
			{ 1, -1},
			{ 1,  0},
			{ 1,  1},
		};

		auto canBeOccupied = std::none_of(dirs.cbegin(), dirs.cend(), [&oldLayout, x, y](const auto& dir) -> bool
		{
			return IsLinearlyOccupied(oldLayout, x, y, std::get<0>(dir), std::get<1>(dir));
		});

		newLayout[y][x] = canBeOccupied ? Cell::Occupied : Cell::Empty;
		return canBeOccupied;
	}

	auto LinearRule::IsLinearlyOccupied(const SeatLayout& layout, int x, int y, int dirX, int dirY) -> bool
	{
		x += dirX;
		y += dirY;

		while (y >= 0 && y < layout.GetRowCount() && x >= 0 && x < layout[y].size())
		{
			switch (layout[y][x])
			{
			case Cell::Occupied:
				return true;
			case Cell::Empty:
				return false;
			}

			x += dirX;
			y += dirY;
		}

		return false;
	}
}

void Day11::SolutionPartOne(const std::string& inputFilePath)
{
	auto pOldLayout = std::make_unique<SeatLayout>();
	auto pNewLayout = std::make_unique<SeatLayout>();
	auto rule = AdjacentRule();
	auto count = 0;
	auto input = std::ifstream(inputFilePath);

	for (std::string line; std::getline(input, line); )
	{
		pOldLayout->AppendRow(line);
		pNewLayout->AppendRow(line);
	}

	for (; rule.ApplyRule(*pNewLayout, *pOldLayout); ++count)
	{
		pNewLayout.swap(pOldLayout);
	}

	std::cout << "After " << count << " times of applying rules, the layout is stabilized." << std::endl;
	std::cout << "The occupied seat count by then is: " << pOldLayout->GetOccupiedCount() << std::endl;
}

void Day11::SolutionPartTwo(const std::string& inputFilePath)
{
	auto pOldLayout = std::make_unique<SeatLayout>();
	auto pNewLayout = std::make_unique<SeatLayout>();
	auto rule = LinearRule();
	auto count = 0;
	auto input = std::ifstream(inputFilePath);

	for (std::string line; std::getline(input, line); )
	{
		pOldLayout->AppendRow(line);
		pNewLayout->AppendRow(line);
	}

	for (; rule.ApplyRule(*pNewLayout, *pOldLayout); ++count)
	{
		pNewLayout.swap(pOldLayout);
	}

	std::cout << "After " << count << " times of applying rules, the layout is stabilized." << std::endl;
	std::cout << "The occupied seat count by then is: " << pOldLayout->GetOccupiedCount() << std::endl;
}

