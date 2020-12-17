#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include "Day16.h"

namespace
{
	template <class T>
	struct Interval
	{
		T min;
		T max;

		Interval(T _min, T _max) : min(_min), max(_max) { assert(min <= max); }

		inline auto Contains(T val) const -> bool
		{
			return min <= val && val <= max;
		}
	};

	template <class T>
	class Intervals
	{
	public:
		void MergeInterval(Interval<T> interval);
		auto Contains(T val) const -> bool;
		
	private:
		std::vector<Interval<T>> m_intervals;
	};

	template <class T>
	auto Intervals<T>::Contains(T val) const -> bool
	{
		// Can be optimized using binary search
		return std::any_of(std::cbegin(m_intervals), std::cend(m_intervals), [val](const auto& interval) { return interval.Contains(val); });
	}

	template <class T>
	void Intervals<T>::MergeInterval(Interval<T> interval)
	{
		auto lowerIt = std::upper_bound(std::begin(m_intervals),
		                                std::end(m_intervals),
										interval.min,
										[](T value, const auto& intv)
										{
										    return intv.max > value;
										});

		auto upperIt = std::lower_bound(std::begin(m_intervals),
		                                std::end(m_intervals),
										interval.max,
										[](const auto& intv, T value)
										{
										    return intv.min < value;
										});

		if (lowerIt == upperIt)
		{
			m_intervals.insert(lowerIt, std::move(interval));
		}

		else
		{
			lowerIt->min = std::min(lowerIt->min, interval.min);
			lowerIt->max = std::max(std::prev(upperIt)->max, interval.max);
			m_intervals.erase(lowerIt + 1, upperIt);
		}
	}

	auto ParseNumberStr(const std::string& str) -> std::vector<int>
	{
		auto result = std::vector<int>();
		auto stream = std::stringstream(str);

		for (auto subStr = std::string(); std::getline(stream, subStr, ',');)
		{
			result.push_back(std::stoi(subStr));
		}

		return result;
	}

	template <class T>
	auto IsValidTicket(const std::vector<T>& ticket, const Intervals<T>& intervals) -> bool
	{
		return std::all_of(std::cbegin(ticket), std::cend(ticket), [&intervals](T i) { return intervals.Contains(i); });
	}
}

void Day16::SolutionPartOne(const std::string& inputFilePath)
{
	const auto regex = std::regex(R"rgx(^.*: (\d+)-(\d+) or (\d+)-(\d+)$)rgx");
	auto input = std::ifstream(inputFilePath);
	auto intervals = Intervals<int>();
	auto match = std::smatch();
	auto total = 0;

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, regex))
		{
			auto min1 = std::stoi(match[1].str());
			auto max1 = std::stoi(match[2].str());
			auto min2 = std::stoi(match[3].str());
			auto max2 = std::stoi(match[4].str());

			intervals.MergeInterval(Interval<int>{ min1, max1 });
			intervals.MergeInterval(Interval<int>{ min2, max2 });
		}

		else if (line.length() > 0 && std::isdigit(line[0]))
		{
			auto stream = std::stringstream(line);

			for (auto subStr = std::string(); std::getline(stream, subStr, ',');)
			{
				auto num = std::stoi(subStr);
				if (!intervals.Contains(num))
				{
					total += num;
				}
			}
		}
	}

	std::cout << "Ticket scanning error rate: " << total << std::endl;
}

void Day16::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto regex = std::regex(R"rgx(^(.*): (\d+)-(\d+) or (\d+)-(\d+)$)rgx");
	auto input = std::ifstream(inputFilePath);
	auto intervals = Intervals<int>();
	auto fieldIntervals = std::unordered_map<std::string, Intervals<int>>();
	auto match = std::smatch();
	auto yourTicket = std::vector<int>();
	auto allTickets = std::vector<std::vector<int>>();
	auto yourTicketFlag = false;

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, regex))
		{
			auto name = match[1].str();
			auto min1 = std::stoi(match[2].str());
			auto max1 = std::stoi(match[3].str());
			auto min2 = std::stoi(match[4].str());
			auto max2 = std::stoi(match[5].str());

			intervals.MergeInterval(Interval<int>{ min1, max1 });
			intervals.MergeInterval(Interval<int>{ min2, max2 });

			auto tempIntervals = Intervals<int>();
			tempIntervals.MergeInterval(Interval<int>{ min1, max1 });
			tempIntervals.MergeInterval(Interval<int>{ min2, max2 });
			fieldIntervals[name] = std::move(tempIntervals);
		}

		else if (line.length() > 0 && std::isdigit(line[0]))
		{
			if (yourTicketFlag)
			{
				yourTicket = ParseNumberStr(line);
				allTickets.push_back(yourTicket);
			}
			else
			{
				allTickets.push_back(ParseNumberStr(line));
			}
		}

		yourTicketFlag = line.compare("your ticket:") == 0;
	}

	allTickets.erase(
		std::remove_if(
			allTickets.begin(),
			allTickets.end(),
			[&intervals](const auto& ticket) { return !IsValidTicket(ticket, intervals); }),
			allTickets.end());

	auto fieldCount = yourTicket.size();
	auto fieldIndices = std::unordered_map<std::string, std::vector<int>>();
	auto product = static_cast<unsigned long long>(1);

	for (const auto& [fieldName, fieldInterval] : fieldIntervals)
	{
		for (auto j = 0; j < fieldCount; ++j)
		{
			if (std::all_of(allTickets.cbegin(), allTickets.cend(), [&](const auto& ticket)
			{
				return fieldInterval.Contains(ticket[j]);
			}))
			{
				fieldIndices[fieldName].push_back(j);
			}
		}
	}

	for (auto i = 0; i < fieldCount; ++i)
	{
		auto it = std::find_if(fieldIndices.begin(), fieldIndices.end(), [i](const auto& pair)
		{
			return pair.second.size() == 1;
		});

		const auto& [name, indices] = *it;
		auto index = indices[0];

		if (name.find("departure") != std::string::npos)
		{
			product *= yourTicket[index];
		}

		std::for_each(fieldIndices.begin(), fieldIndices.end(), [index](auto& pair)
		{
			auto& indices = pair.second;
			indices.erase(std::remove(indices.begin(), indices.end(), index), indices.end());
		});
	}

	std::cout << "Total product: " << product << std::endl;
}

