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

	class Program
	{
	public:
		Program(const std::string& programFile);
		void ExecuteNoLoop(Accumulator& accumulator, InstructionCounter& counter);
		void ExecuteBreakOnLast(Accumulator& accumulator, InstructionCounter& counter);
		std::vector<Instruction> instructions;

	private:
		void ExecuteInstruction(const Instruction& instruction, Accumulator& accumulator, InstructionCounter& counter);
	};

	class VirtualMachine
	{
	public:
		void ExecuteProgram(Program& program, int startInstructionIndex = 0);
		inline auto InspectAccumulator() const -> Accumulator { return accumulator; }
		inline auto InspectInstructionCounter() const -> InstructionCounter { return counter; }

	private:
		Accumulator accumulator;
		InstructionCounter counter;
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

	void Program::ExecuteNoLoop(Accumulator& accumulator, InstructionCounter& counter)
	{
		auto executedIndices = std::unordered_set<int>();
		auto index = counter;

		while (executedIndices.find(counter) == executedIndices.end()
			&& counter >= 0 && counter < instructions.size())
		{
			executedIndices.insert(counter);
			ExecuteInstruction(instructions[counter], accumulator, counter);
		}
	}

	void Program::ExecuteInstruction(const Instruction& instruction, Accumulator& accumulator, InstructionCounter& counter)
	{
		assert(instruction.type != Instruction::Type::NONE);

		switch (instruction.type)
		{
		case Instruction::Type::NOP:
			++counter;
			break;
		case Instruction::Type::ACC:
			++counter;
			accumulator += instruction.value;
			break;
		case Instruction::Type::JMP:
			counter += instruction.value;
			break;
		}
	}

	void VirtualMachine::ExecuteProgram(Program& program, int startInstructionIndex /*= 0*/)
	{
		accumulator = 0;
		counter = startInstructionIndex;
		program.ExecuteNoLoop(accumulator, counter);
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

