#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include "Day12.h"

namespace
{
	static inline auto DegreesToRadians(double degrees) -> double
	{
		static const auto kPi = std::acos(-1);
		return degrees * kPi / 180;
	}

	struct Instruction
	{
		enum class Type
		{
			None,
			MoveForward,
			MoveNorth,
			MoveSouth,
			MoveEast,
			MoveWest,
			TurnLeft,
			TurnRight,
		};

		Type type;
		int value;

		Instruction(const std::string&);
	};

	struct Coordinate
	{
		int x;
		int y;
	};

	struct Ship
	{
		Coordinate coordinate;
		int angle;
	};

	class NavSystem
	{
	public:
		void ExecuteInstruction(Ship& ship, const Instruction& instruction);

	private:
		void MoveForward(Ship& ship, int value);
		void MoveNorth(Ship& ship, int value);
		void MoveSouth(Ship& ship, int value);
		void MoveEast(Ship& ship, int value);
		void MoveWest(Ship& ship, int value);
		void TurnLeft(Ship& ship, int value);
		void TurnRight(Ship& ship, int value);
	};

	class WayPointSystem
	{
	public:
		WayPointSystem(int x, int y) : m_wayPoint{x, y} {}
		void ExecuteInstruction(Ship& ship, const Instruction& instruction);

	private:
		void MoveForward(Ship& ship, int value);
		void MoveNorth(Ship& ship, int value);
		void MoveSouth(Ship& ship, int value);
		void MoveEast(Ship& ship, int value);
		void MoveWest(Ship& ship, int value);
		void TurnWayPoint(int value);

		Coordinate m_wayPoint;
	};

	Instruction::Instruction(const std::string& str)
	{
		static const auto rgx = std::regex(R"rgx(^([FNSEWLR])(\d+)$)rgx");
		auto match = std::smatch();
		if (std::regex_match(str, match, rgx))
		{
			auto typeChar = match[1].str()[0];
			value = std::stoi(match[2].str());

			switch (typeChar)
			{
			case 'F':
				type = Type::MoveForward;
				break;
			case 'N':
				type = Type::MoveNorth;
				break;
			case 'S':
				type = Type::MoveSouth;
				break;
			case 'E':
				type = Type::MoveEast;
				break;
			case 'W':
				type = Type::MoveWest;
				break;
			case 'L':
				type = Type::TurnLeft;
				break;
			case 'R':
				type = Type::TurnRight;
				break;
			}
		}
	}

	void NavSystem::ExecuteInstruction(Ship& ship, const Instruction& instruction)
	{
		switch (instruction.type)
		{
		case Instruction::Type::MoveForward:
			MoveForward(ship, instruction.value);
			break;
		case Instruction::Type::MoveNorth:
			MoveNorth(ship, instruction.value);
			break;
		case Instruction::Type::MoveSouth:
			MoveSouth(ship, instruction.value);
			break;
		case Instruction::Type::MoveEast:
			MoveEast(ship, instruction.value);
			break;
		case Instruction::Type::MoveWest:
			MoveWest(ship, instruction.value);
			break;
		case Instruction::Type::TurnLeft:
			TurnLeft(ship, instruction.value);
			break;
		case Instruction::Type::TurnRight:
			TurnRight(ship, instruction.value);
			break;
		}
	}

	void NavSystem::MoveForward(Ship& ship, int value)
	{
		static const auto kPi = std::acos(-1);
		auto rad = DegreesToRadians(ship.angle);
		auto sin = std::sin(rad);
		auto cos = std::cos(rad);

		ship.coordinate.x += std::round(value * cos);
		ship.coordinate.y += std::round(value * sin);
	}

	void NavSystem::MoveNorth(Ship& ship, int value)
	{
		ship.coordinate.y += value;
	}

	void NavSystem::MoveSouth(Ship& ship, int value)
	{
		ship.coordinate.y -= value;
	}

	void NavSystem::MoveEast(Ship& ship, int value)
	{
		ship.coordinate.x += value;
	}

	void NavSystem::MoveWest(Ship& ship, int value)
	{
		ship.coordinate.x -= value;
	}

	void NavSystem::TurnLeft(Ship& ship, int value)
	{
		ship.angle += value;
	}

	void NavSystem::TurnRight(Ship& ship, int value)
	{
		ship.angle -= value;
	}

	void WayPointSystem::ExecuteInstruction(Ship& ship, const Instruction& instruction)
	{
		switch (instruction.type)
		{
		case Instruction::Type::MoveForward:
			MoveForward(ship, instruction.value);
			break;
		case Instruction::Type::MoveNorth:
			MoveNorth(ship, instruction.value);
			break;
		case Instruction::Type::MoveSouth:
			MoveSouth(ship, instruction.value);
			break;
		case Instruction::Type::MoveEast:
			MoveEast(ship, instruction.value);
			break;
		case Instruction::Type::MoveWest:
			MoveWest(ship, instruction.value);
			break;
		case Instruction::Type::TurnLeft:
			TurnWayPoint(instruction.value);
			break;
		case Instruction::Type::TurnRight:
			TurnWayPoint(-instruction.value);
			break;
		}
	}

	void WayPointSystem::MoveForward(Ship& ship, int value)
	{
		ship.coordinate.x += m_wayPoint.x * value;
		ship.coordinate.y += m_wayPoint.y * value;
	}

	void WayPointSystem::MoveNorth(Ship& ship, int value)
	{
		m_wayPoint.y += value;
	}

	void WayPointSystem::MoveSouth(Ship& ship, int value)
	{
		m_wayPoint.y -= value;
	}

	void WayPointSystem::MoveEast(Ship& ship, int value)
	{
		m_wayPoint.x += value;
	}

	void WayPointSystem::MoveWest(Ship& ship, int value)
	{
		m_wayPoint.x -= value;
	}

	void WayPointSystem::TurnWayPoint(int value)
	{
		static const auto kPi = std::acos(-1);
		auto rad = DegreesToRadians(value);
		auto sin = std::sin(rad);
		auto cos = std::cos(rad);

		auto wayPointX = m_wayPoint.x;
		auto wayPointY = m_wayPoint.y;
		m_wayPoint.x = std::round(wayPointX * cos - wayPointY * sin);
		m_wayPoint.y = std::round(wayPointX * sin + wayPointY * cos);
	}
}

void Day12::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream{inputFilePath};
	auto ship = Ship{};
	auto nav = NavSystem{};

	for (auto line = std::string(); std::getline(input, line); )
	{
		auto instruction = Instruction{line};
		nav.ExecuteInstruction(ship, instruction);
	}

	std::cout << "Final coordinate X: " << ship.coordinate.x << " Y: " << ship.coordinate.y << std::endl;
	std::cout << "The Manhattan distance is: " << std::abs(ship.coordinate.x) + std::abs(ship.coordinate.y) << std::endl;
}

void Day12::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream{ inputFilePath };
	auto ship = Ship{};
	auto sys = WayPointSystem{10, 1};

	for (auto line = std::string(); std::getline(input, line); )
	{
		auto instruction = Instruction{ line };
		sys.ExecuteInstruction(ship, instruction);
	}

	std::cout << "Final coordinate X: " << ship.coordinate.x << " Y: " << ship.coordinate.y << std::endl;
	std::cout << "The Manhattan distance is: " << std::abs(ship.coordinate.x) + std::abs(ship.coordinate.y) << std::endl;
}
