#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include "Day21.h"

namespace
{
	using Allergent = std::string;
	using Ingredient = std::string;

	template <class Key, class Hash, class KeyEqual, class Alloc, class Pred>
	typename std::unordered_set<Key, Hash, KeyEqual, Alloc>::size_type
		erase_if(std::unordered_set<Key, Hash, KeyEqual, Alloc>& c, Pred pred)
	{
		auto old_size = c.size();
		for (auto i = c.begin(), last = c.end(); i != last; ) {
			if (pred(*i)) {
				i = c.erase(i);
			}
			else {
				++i;
			}
		}
		return old_size - c.size();
	}

	template <class It, class DelimiterT, class Pred>
	auto StringJoin(It begin, It end, const DelimiterT& delimiter, Pred pred) -> std::string
	{
		auto stream = std::ostringstream();

		if (begin != end)
		{
			stream << pred(*begin++);
		}

		while (begin != end)
		{
			stream << delimiter;
			stream << pred(*begin++);
		}

		return stream.str();
	}

	auto SplitStrings(const std::string& str, char delimiter) -> std::unordered_set<std::string>
	{
		auto result = std::unordered_set<std::string>();
		auto stream = std::stringstream(str);

		for (auto subStr = std::string(); std::getline(stream, subStr, delimiter);)
		{
			subStr.erase(std::remove(subStr.begin(), subStr.end(), ' '));
			result.insert(subStr);
		}

		return result;
	}
}

void Day21::SolutionPartOne(const std::string& inputFilePath)
{
	const auto regex = std::regex(R"rgx(^(.+) \(contains (.+)\)$)rgx");
	auto input = std::ifstream(inputFilePath);
	auto match = std::smatch();
	auto ingredientCounter = std::unordered_map<Ingredient, int>();
	auto lookupTable = std::unordered_map<Allergent, std::unordered_set<Ingredient>>();
	auto allIngredients = std::unordered_set<Ingredient>();

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, regex))
		{
			auto ingredientStr = match[1].str();
			auto allergenStr = match[2].str();

			auto ingredients = SplitStrings(ingredientStr, ' ');
			auto allergents = SplitStrings(allergenStr, ',');

			allIngredients.insert(ingredients.cbegin(), ingredients.cend());

			for (const auto& ingredient : ingredients)
			{
				++ingredientCounter[ingredient];
			}

			for (const auto& allergent : allergents)
			{
				if (lookupTable.find(allergent) == lookupTable.end())
				{
					lookupTable[allergent].insert(ingredients.cbegin(), ingredients.cend());
				}

				else
				{
					auto& ingredientCandidates = lookupTable[allergent];
					erase_if(ingredientCandidates, [&ingredients](auto& ingre) 
					{
						return ingredients.find(ingre) == ingredients.end();
					});
				}
			}
		}
	}

	auto it = std::find_if(lookupTable.cbegin(), lookupTable.cend(), [](const auto& pair) { return pair.second.size() == 1; });
	auto identifiedIngredients = std::unordered_set<Ingredient>();

	while (it != lookupTable.cend())
	{
		const auto& [allergent, ingredients] = *it;
		const auto& ingredient = *ingredients.cbegin();

		for (auto& [key, value] : lookupTable)
		{
			if (key != allergent)
			{
				value.erase(ingredient);
			}
		}

		identifiedIngredients.insert(ingredient);

		it = std::find_if(lookupTable.cbegin(), lookupTable.cend(), [&identifiedIngredients](const auto& pair)
		{
			return pair.second.size() == 1 && identifiedIngredients.find(*pair.second.cbegin()) == identifiedIngredients.end();
		});
	}

	auto totalCount = 0;

	for (const auto& [key, value] : ingredientCounter)
	{
		if (identifiedIngredients.find(key) == identifiedIngredients.end())
		{
			totalCount += value;
		}
	}

	std::cout << "Total appearance: " << totalCount << std::endl;
}

void Day21::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto regex = std::regex(R"rgx(^(.+) \(contains (.+)\)$)rgx");
	auto input = std::ifstream(inputFilePath);
	auto match = std::smatch();
	auto lookupTable = std::unordered_map<Allergent, std::unordered_set<Ingredient>>();
	auto allIngredients = std::unordered_set<Ingredient>();

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, regex))
		{
			auto ingredientStr = match[1].str();
			auto allergenStr = match[2].str();

			auto ingredients = SplitStrings(ingredientStr, ' ');
			auto allergents = SplitStrings(allergenStr, ',');

			allIngredients.insert(ingredients.cbegin(), ingredients.cend());

			for (const auto& allergent : allergents)
			{
				if (lookupTable.find(allergent) == lookupTable.end())
				{
					lookupTable[allergent].insert(ingredients.cbegin(), ingredients.cend());
				}

				else
				{
					auto& ingredientCandidates = lookupTable[allergent];
					erase_if(ingredientCandidates, [&ingredients](auto& ingre) 
					{
						return ingredients.find(ingre) == ingredients.end();
					});
				}
			}
		}
	}

	auto it = std::find_if(lookupTable.cbegin(), lookupTable.cend(), [](const auto& pair) { return pair.second.size() == 1; });
	auto identifiedIngredients = std::unordered_map<Ingredient, Allergent>();

	while (it != lookupTable.cend())
	{
		const auto& [allergent, ingredients] = *it;
		const auto& ingredient = *ingredients.cbegin();

		for (auto& [key, value] : lookupTable)
		{
			if (key != allergent)
			{
				value.erase(ingredient);
			}
		}

		identifiedIngredients.insert(std::make_pair(ingredient, allergent));

		it = std::find_if(lookupTable.cbegin(), lookupTable.cend(), [&identifiedIngredients](const auto& pair)
		{
			return pair.second.size() == 1 && identifiedIngredients.find(*pair.second.cbegin()) == identifiedIngredients.end();
		});
	}

	auto allergeIngredientPairs = std::vector<std::pair<Allergent, Ingredient>>();
	std::transform(identifiedIngredients.cbegin(),
	               identifiedIngredients.cend(),
				   std::back_inserter(allergeIngredientPairs),
				   [&identifiedIngredients](const auto& pair)
				   {
				       return std::make_pair(pair.second, pair.first);
				   });

	std::sort(allergeIngredientPairs.begin(), allergeIngredientPairs.end(), [](const auto& a, const auto& b)
	{
		return a.first.compare(b.first) < 0;
	});

	auto result = StringJoin(allergeIngredientPairs.begin(),
	                         allergeIngredientPairs.end(),
							 ',',
							 [](const auto& pair)
							 {
							     return pair.second;
							 });

	std::cout << "Final string: " << result << std::endl;
}