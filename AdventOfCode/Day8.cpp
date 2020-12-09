#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <unordered_set>
#include <algorithm>
#include <assert.h>
#include "Day8.h"

namespace
{
	using Accumulator = int;
	using InstructionCounter = int;

	struct Instruction
	{
		const char* NOP = "nop";
		const char* ACC = "acc";
		const char* JMP = "jmp";
		enum class Type { NONE, NOP, ACC, JMP, };
		Type type;
		int value;

		Instruction(const std::string& str);
	};

	struct Program
	{
		Program(const std::string& programFile);
		std::vector<Instruction> instructions;
	};

	class VirtualMachine
	{
	public:
		void ExecuteProgram(const Program& program, int startInstructionIndex = 0);
		inline auto InspectAccumulator() const -> Accumulator { return m_accumulator; }
		inline auto InspectInstructionCounter() const -> InstructionCounter { return m_counter; }

	private:
		void ExecuteInstruction(const Instruction& instruction);
		Accumulator m_accumulator;
		InstructionCounter m_counter;
	};

	Instruction::Instruction(const std::string& str)
	{
		const auto rgx = std::regex(R"rgx(^([a-z]{3}) ([+-])(\d*)$)rgx");
		auto match = std::smatch();
		if (std::regex_match(str, match, rgx))
		{
			auto instruction = match[1].str();
			auto sign = match[2].str()[0];
			auto number = std::stoi(match[3].str());

			if (instruction.compare(NOP) == 0)
			{
				type = Type::NOP;
			}
			else if (instruction.compare(ACC) == 0)
			{
				type = Type::ACC;
			}
			else if (instruction.compare(JMP) == 0)
			{
				type = Type::JMP;
			}

			value = (sign == '+' ? 1 : -1) * number;
		}
	}

	Program::Program(const std::string& programFile)
	{
		auto input = std::ifstream(programFile);
		for (auto line = std::string(); std::getline(input, line); )
		{
			instructions.emplace_back(line);
		}
	}

	void VirtualMachine::ExecuteProgram(const Program& program, int startInstructionIndex /*= 0*/)
	{
		auto executedIndices = std::unordered_set<int>();
		const auto& instructions = program.instructions;
		m_accumulator = 0;
		m_counter = startInstructionIndex;

		while (executedIndices.find(m_counter) == executedIndices.end()
			&& m_counter >= 0 && m_counter < instructions.size())
		{
			executedIndices.insert(m_counter);
			ExecuteInstruction(instructions[m_counter]);
		}
	}

	void VirtualMachine::ExecuteInstruction(const Instruction& instruction)
	{
		assert(instruction.type != Instruction::Type::NONE);

		switch (instruction.type)
		{
		case Instruction::Type::NOP:
			++m_counter;
			break;
		case Instruction::Type::ACC:
			++m_counter;
			m_accumulator += instruction.value;
			break;
		case Instruction::Type::JMP:
			m_counter += instruction.value;
			break;
		}
	}

}

void Day8::SolutionPartOne(const std::string& inputFilePath)
{
	auto vMachine = VirtualMachine();
	auto program = Program(inputFilePath);
	vMachine.ExecuteProgram(program);

	std::cout << "Accumulator value is: " << vMachine.InspectAccumulator() << std::endl;
}

void Day8::SolutionPartTwo(const std::string& inputFilePath)
{
	auto vMachine = VirtualMachine();
	auto program = Program(inputFilePath);
	auto& instructions = program.instructions;
	auto lastIdx = -1;

	auto FlipInstruction = [](auto& instructions, auto& idx, auto oldType, auto newType)
	{
		// Recover the last swap
		if (idx >= 0)
		{
			instructions[idx].type = oldType;
		}
		idx = std::distance(instructions.cbegin(),
		                    std::find_if(std::next(instructions.cbegin(), idx + 1),
		                                 instructions.cend(),
		                                 [oldType](const auto& ins) -> bool { return ins.type == oldType; }));
		if (idx >= 0 && idx < instructions.size())
		{
			instructions[idx].type = newType;
		}
	};

	// Flipping NOP to JMP
	do 
	{
		FlipInstruction(instructions, lastIdx, Instruction::Type::NOP, Instruction::Type::JMP);
		vMachine.ExecuteProgram(program);
		auto counter = vMachine.InspectInstructionCounter();

		if (counter == instructions.size())
		{
			std::cout << "Program terminates when changing the " << lastIdx << "th NOP to JMP" << std::endl;
			std::cout << "Accumulator value is: " << vMachine.InspectAccumulator() << std::endl;
		}
	} while (lastIdx >= 0 && lastIdx < instructions.size());

	// Flipping JMP to NOP
	lastIdx = -1;
	do
	{
		FlipInstruction(instructions, lastIdx, Instruction::Type::JMP, Instruction::Type::NOP);
		vMachine.ExecuteProgram(program);
		auto counter = vMachine.InspectInstructionCounter();

		if (counter == instructions.size())
		{
			std::cout << "Program terminates when changing the " << lastIdx << "th JMP to NOP" << std::endl;
			std::cout << "Accumulator value is: " << vMachine.InspectAccumulator() << std::endl;
		}
	} while (lastIdx >= 0 && lastIdx < instructions.size());
}

