#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

#include "Day2.h"

void Day2::SolutionPartOne(const std::string& inputFilePath)
{
	auto count = 0;
	auto input = std::ifstream(inputFilePath);

	for (std::string line; std::getline(input, line); )
	{
		auto rgx = std::regex(R"rgx(^(\d+)-(\d+) ([a-z]) ([a-z]+)$)rgx");
		auto match = std::smatch();

		if (std::regex_match(line, match, rgx))
		{
			auto min = std::stoi(match[1]);
			auto max = std::stoi(match[2]);
			auto ch = match[3].str()[0];
			auto pwd = match[4].str();
			auto charCount = std::count(pwd.begin(), pwd.end(), ch);

			count += (charCount >= min) && (charCount <= max);
		}
	}

	std::cout << "Total valid password count: " << count << std::endl;
}

void Day2::SolutionPartTwo(const std::string& inputFilePath)
{
	auto count = 0;
	auto input = std::ifstream(inputFilePath);

	for (std::string line; std::getline(input, line); )
	{
		auto rgx = std::regex(R"rgx(^(\d+)-(\d+) ([a-z]) ([a-z]+)$)rgx");
		auto match = std::smatch();

		if (std::regex_match(line, match, rgx))
		{
			const auto StrContains = [](const auto& str, auto ch, auto pos) -> bool
			{
				return pos < str.length() && pos >= 0 && ch == str[pos];
			};

			auto firstPos = std::stoi(match[1]);
			auto secondPos = std::stoi(match[2]);
			auto ch = match[3].str()[0];
			auto pwd = match[4].str();

			count += StrContains(pwd, ch, firstPos - 1) ^ StrContains(pwd, ch, secondPos - 1);
		}
	}

	std::cout << "Total valid password count: " << count << std::endl;
}