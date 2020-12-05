#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "Day4.h"

namespace
{
	auto HasAllRequiredFields(const std::vector<std::string>& strs) -> bool
	{
		const std::string kFields[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
		auto fields = std::unordered_set<std::string>();

		for (const auto& str : strs)
		{
			auto iss = std::istringstream(str);
			for (auto kvp = std::string(); iss >> kvp; )
			{
				auto field = kvp.substr(0, kvp.find(':'));
				fields.insert(std::move(field));
			}
		}

		return std::all_of(std::cbegin(kFields),
		                   std::cend(kFields),
		                   [&fields = static_cast<const std::unordered_set<std::string>&>(fields)](const auto& field)
		                   { return fields.find(field) != fields.cend(); });
	}

	auto IsValidBirthYear(const std::string& value) -> bool
	{
		const auto kYearMin = 1920;
		const auto kYearMax = 2002;
		auto year = std::stoi(value);
		return year >= kYearMin && year <= kYearMax;
	}

	auto IsValidIssueYear(const std::string& value) -> bool
	{
		const auto kYearMin = 2010;
		const auto kYearMax = 2020;
		auto year = std::stoi(value);
		return year >= kYearMin && year <= kYearMax;
	}

	auto IsValidExpirationYear(const std::string& value) -> bool
	{
		const auto kYearMin = 2020;
		const auto kYearMax = 2030;
		auto year = std::stoi(value);
		return year >= kYearMin && year <= kYearMax;
	}

	auto IsValidHeight(const std::string& value) -> bool
	{
		const auto kCmMin = 150;
		const auto kCmMax = 193;
		const auto kInMin = 59;
		const auto kInMax = 76;
		auto pos = value.find("cm");

		if (pos != std::string::npos)
		{
			auto num = std::stoi(value.substr(0, pos));
			return num >= kCmMin && num <= kCmMax;
		}

		else
		{
			pos = value.find("in");
			if (pos != std::string::npos)
			{
				auto num = std::stoi(value.substr(0, pos));
				return num >= kInMin && num <= kInMax;
			}
		}

		return false;
	}

	auto IsValidHairColor(const std::string& value) -> bool
	{
		const auto kHairColorRegex = std::regex(R"rgx(^#[a-f0-9]{6}$)rgx");
		auto match = std::smatch();
		return std::regex_match(value, match, kHairColorRegex);
	}

	auto IsValidEyeColor(const std::string& value) -> bool
	{
		const std::string kEyeColors[] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
		return std::find(std::cbegin(kEyeColors), std::cend(kEyeColors), value) != std::cend(kEyeColors);
	}

	auto IsValidPassportId(const std::string& value) -> bool
	{
		const auto kPasswordRegex = std::regex(R"rgx(^(\d){9}$)rgx");
		auto match = std::smatch();
		return std::regex_match(value, match, kPasswordRegex);
	}

	auto IsValidKvps(const std::unordered_map<std::string, std::string>& kvps) -> bool
	{
		using FieldValidationFunc = auto (*)(const std::string&) -> bool;
		const std::unordered_map<std::string, FieldValidationFunc> kVAlidationFuncMap = 
		{
			{ "byr", IsValidBirthYear },
			{ "iyr", IsValidIssueYear },
			{ "eyr", IsValidExpirationYear },
			{ "hgt", IsValidHeight },
			{ "hcl", IsValidHairColor },
			{ "ecl", IsValidEyeColor },
			{ "pid", IsValidPassportId },
		};

		return std::all_of(kvps.cbegin(),
		                   kvps.cend(),
		                   [&](const auto& kvp)
						   {
								auto it = kVAlidationFuncMap.find(kvp.first);
								if (it != kVAlidationFuncMap.end())
								{
									return it->second(kvp.second);
								}
								return true;
						   });
	}

	auto IsValidPassport(const std::vector<std::string>& strs) -> bool
	{
		const std::string kFields[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
		auto kvps = std::unordered_map<std::string, std::string>();

		for (const auto& str : strs)
		{
			auto iss = std::istringstream(str);
			for (auto kvpStr = std::string(); iss >> kvpStr; )
			{
				const auto delimiterPos = kvpStr.find(':');
				auto field = kvpStr.substr(0, delimiterPos);
				auto value = kvpStr.substr(delimiterPos + 1);

				kvps.insert({std::move(field), std::move(value)});
			}
		}

		auto containsAllFields = std::all_of(std::cbegin(kFields),
		                                     std::cend(kFields),
		                                     [&kvps = static_cast<const std::unordered_map<std::string, std::string>&>(kvps)](const auto& field)
		                                     { return kvps.find(field) != kvps.cend(); });

		return containsAllFields && IsValidKvps(kvps);
	}
}

void Day4::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto strs = std::vector<std::string>();
	auto count = 0;

	for (std::string line; std::getline(input, line); )
	{
		if (!line.empty())
		{
			strs.push_back(std::move(line));
		}

		else
		{
			count += HasAllRequiredFields(strs);
			strs.clear();
		}
	}

	count += HasAllRequiredFields(strs);

	std::cout << "Valid passport count: " << count << std::endl;
}

void Day4::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto strs = std::vector<std::string>();
	auto count = 0;

	for (std::string line; std::getline(input, line); )
	{
		if (!line.empty())
		{
			strs.push_back(std::move(line));
		}

		else
		{
			count += IsValidPassport(strs);
			strs.clear();
		}
	}

	count += IsValidPassport(strs);

	std::cout << "Valid passport count: " << count << std::endl;
}
