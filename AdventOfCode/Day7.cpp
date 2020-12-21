#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "Day7.h"

namespace
{
	using Weight = int;
	using Graph = std::unordered_map<std::string, std::unordered_set<std::string>>;
	using WeightedGraph = std::unordered_map<std::string, std::unordered_map<std::string, Weight>>;

	void BuildDirectionalGraph(Graph& graph, const std::string& rule)
	{
		auto parentBag = rule.substr(0, rule.find(" bags"));
		auto subStrStart = rule.find("contain ") + std::string("contain ").length();
		auto subStr = rule.substr(subStrStart, rule.length() - subStrStart + 1);		
		auto stream = std::stringstream(subStr);

		for (auto subSubStr = std::string(); std::getline(stream, subSubStr, ','); )
		{
			const auto rgx = std::regex(R"rgx(^ ?(\d)+ ([a-z ]+) bags?(, )?(.)?$)rgx");
			auto match = std::smatch();
			if (std::regex_match(subSubStr, match, rgx))
			{
				auto childBag = match[2].str();
				graph[childBag].insert(parentBag);
			}
		}
	}

	void BuildWeightedDirectionalGraph(WeightedGraph& graph, const std::string& rule)
	{
		auto parentBag = rule.substr(0, rule.find(" bags"));
		auto subStrStart = rule.find("contain ") + std::string("contain ").length();
		auto subStr = rule.substr(subStrStart, rule.length() - subStrStart + 1);
		auto stream = std::stringstream(subStr);

		for (auto subSubStr = std::string(); std::getline(stream, subSubStr, ','); )
		{
			const auto rgx = std::regex(R"rgx(^ ?(\d)+ ([a-z ]+) bags?(, )?(.)?$)rgx");
			auto match = std::smatch();
			if (std::regex_match(subSubStr, match, rgx))
			{
				auto count = std::stoi(match[1].str());
				auto childBag = match[2].str();
				graph[parentBag].insert({childBag, count});
			}
		}
	}

	auto CountAvailableBagTypes(const Graph& graph, const std::string& bagType) -> int
	{
		auto parentQueue = std::deque<std::string>();
		auto visitedTypes = std::unordered_set<std::string>();
		auto count = 0;

		for (const auto& parent : graph.at(bagType))
		{
			parentQueue.push_back(parent);
		}

		for (; !parentQueue.empty(); ++count)
		{
			const auto& parent = parentQueue.front();
			visitedTypes.insert(parent);

			if (graph.find(parent) != graph.end())
			{
				for (const auto& grandParent : graph.at(parent))
				{
					if (visitedTypes.find(grandParent) == visitedTypes.end())
					{
						parentQueue.push_back(grandParent);
						visitedTypes.insert(grandParent);
					}
				}
			}

			parentQueue.pop_front();
		}

		return count;
	}

	void DfsSolver(const WeightedGraph& graph, const std::string& curType, int& count, int multiplier)
	{
		if (graph.find(curType) != graph.end())
		{
			const auto& children = graph.at(curType);

			for (const auto& kvp : children)
			{
				DfsSolver(graph, kvp.first, count, multiplier * kvp.second);
				count += multiplier * kvp.second;
			}
		}
	}

	auto CountTotalContainedBags(const WeightedGraph& graph, const std::string& bagType) -> int
	{
		auto count = 0;
		auto multiplier = 1;
		DfsSolver(graph, bagType, count, multiplier);

		return count;
	}	
}


void Day7::SolutionPartOne(const std::string& inputFilePath)
{
	const auto color = "shiny gold";
	auto input = std::ifstream(inputFilePath);
	auto graph = Graph();

	for (std::string line; std::getline(input, line); )
	{
		BuildDirectionalGraph(graph, line);
	}

	std::cout << "Total types that can contain '" << color << "' is " << CountAvailableBagTypes(graph, "shiny gold") << std::endl;
}

void Day7::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto color = "shiny gold";
	auto input = std::ifstream(inputFilePath);
	auto graph = WeightedGraph();

	for (std::string line; std::getline(input, line); )
	{
		BuildWeightedDirectionalGraph(graph, line);
	}

	std::cout << "Total bags that '" << color << "' can contain is " << CountTotalContainedBags(graph, "shiny gold") << std::endl;
}

