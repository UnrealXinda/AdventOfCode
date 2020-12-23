#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <deque>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include "Day22.h"

namespace
{
	using Card = unsigned int;
	using RoundHash = std::size_t;
	using PastRounds = std::unordered_set<RoundHash>;

	enum class GameState
	{
		Ongoing,
		Player1Won,
		Player2Won,
	};

	struct Deck
	{
		std::deque<Card> cards;

		auto ToHash() const -> RoundHash;
	};

	auto Deck::ToHash() const -> RoundHash
	{
		auto seed = cards.size();
		for (auto& i : cards)
		{
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}

	struct Game
	{
		template <class It>
		Game(It begin1, It end1, It begin2, It end2);

		int round;
		GameState state;
		Deck deck1;
		Deck deck2;
	};

	template <class It>
	Game::Game(It begin1, It end1, It begin2, It end2) : round(0), state(GameState::Ongoing)
	{
		deck1.cards.insert(deck1.cards.begin(), begin1, end1);
		deck2.cards.insert(deck2.cards.begin(), begin2, end2);
	}

	class Rule
	{
	public:
		void RunGame(Game& game, int round = 1);
	private:
		void RunGameOneRound(Game& game);
	};

	class RecursiveRule
	{
	public:
		void RunGame(Game& game, PastRounds& pastRounds1, PastRounds& pastRounds2, int round = 1);
	private:
		void RunGameOneRound(Game& game, PastRounds& pastRounds1, PastRounds& pastRounds2);
	};

	void Rule::RunGame(Game& game, int round /*= 1*/)
	{
		for (auto i = 0; i < round; ++i)
		{
			RunGameOneRound(game);
		}
	}

	void Rule::RunGameOneRound(Game& game)
	{
		if (game.state == GameState::Ongoing)
		{
			auto& deck1 = game.deck1;
			auto& deck2 = game.deck2;

			auto card1 = deck1.cards.front(); deck1.cards.pop_front();
			auto card2 = deck2.cards.front(); deck2.cards.pop_front();

			auto& winnerDeck = (card1 > card2) ? deck1 : deck2;
			winnerDeck.cards.push_back(std::max(card1, card2));
			winnerDeck.cards.push_back(std::min(card1, card2));

			++game.round;

			if (deck1.cards.empty())
			{
				game.state = GameState::Player2Won;
			}

			else if (deck2.cards.empty())
			{
				game.state = GameState::Player1Won;
			}
		}
	}

	void RecursiveRule::RunGame(Game& game, PastRounds& pastRounds1, PastRounds& pastRounds2, int round /*= 1*/)
	{
		for (auto i = 0; i < round; ++i)
		{
			RunGameOneRound(game, pastRounds1, pastRounds2);
		}
	}

	void RecursiveRule::RunGameOneRound(Game& game, PastRounds& pastRounds1, PastRounds& pastRounds2)
	{
		if (game.state == GameState::Ongoing)
		{
			auto roundStr1 = game.deck1.ToHash();
			auto roundStr2 = game.deck2.ToHash();

			if (pastRounds1.find(roundStr1) != pastRounds1.end() || pastRounds2.find(roundStr2) != pastRounds2.end())
			{
				game.state = GameState::Player1Won;
				return;
			}
			
			pastRounds1.insert(std::move(roundStr1));
			pastRounds2.insert(std::move(roundStr2));

			auto& deck1 = game.deck1;
			auto& deck2 = game.deck2;

			auto card1 = deck1.cards.front(); deck1.cards.pop_front();
			auto card2 = deck2.cards.front(); deck2.cards.pop_front();

			if (card1 <= deck1.cards.size() && card2 <= deck2.cards.size())
			{
				auto subGame = Game(deck1.cards.cbegin(),
									std::next(deck1.cards.cbegin(), card1),
									deck2.cards.cbegin(),
									std::next(deck2.cards.cbegin(), card2));
				auto subPastRounds1 = PastRounds();
				auto subPastRounds2 = PastRounds();

				while (subGame.state == GameState::Ongoing)
				{
					RunGameOneRound(subGame, subPastRounds1, subPastRounds2);
				}

				auto& winnerDeck = subGame.state == GameState::Player1Won ? deck1 : deck2;
				winnerDeck.cards.push_back(subGame.state == GameState::Player1Won ? card1 : card2);
				winnerDeck.cards.push_back(subGame.state == GameState::Player1Won ? card2 : card1);
			}

			else
			{
				auto& winnerDeck = (card1 > card2) ? deck1 : deck2;
				winnerDeck.cards.push_back(std::max(card1, card2));
				winnerDeck.cards.push_back(std::min(card1, card2));
			}

			++game.round;

			if (deck1.cards.empty())
			{
				game.state = GameState::Player2Won;
			}

			else if (deck2.cards.empty())
			{
				game.state = GameState::Player1Won;
			}
		}
	}
}

void Day22::SolutionPartOne(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto cards1 = std::vector<Card>();
	auto cards2 = std::vector<Card>();
	auto flag = false;

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (line == "Player 2:")
		{
			flag = true;
		}
		else if (!line.empty() && std::isdigit(line[0]))
		{
			auto card = std::stoi(line);
			if (flag)
			{
				cards2.push_back(card);
			}
			else
			{
				cards1.push_back(card);
			}
		}
	}

	auto game = Game(cards1.begin(), cards1.end(), cards2.begin(), cards2.end());
	auto rule = Rule();

	while (game.state == GameState::Ongoing)
	{
		rule.RunGame(game);
	}

	auto num = std::max(game.deck1.cards.size(), game.deck2.cards.size());
	auto& winnerDeck = game.state == GameState::Player1Won ? game.deck1 : game.deck2;
	auto sum = std::accumulate(winnerDeck.cards.cbegin(), winnerDeck.cards.cend(), 0, [&num](auto partialSum, const auto& card)
	{
		return partialSum + card * num--;
	});

	std::cout << "Total sum is: " << sum << std::endl;
}

void Day22::SolutionPartTwo(const std::string& inputFilePath)
{
	auto input = std::ifstream(inputFilePath);
	auto cards1 = std::vector<Card>();
	auto cards2 = std::vector<Card>();
	auto flag = false;

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (line == "Player 2:")
		{
			flag = true;
		}
		else if (!line.empty() && std::isdigit(line[0]))
		{
			auto card = std::stoi(line);
			if (flag)
			{
				cards2.push_back(card);
			}
			else
			{
				cards1.push_back(card);
			}
		}
	}

	auto game = Game(cards1.begin(), cards1.end(), cards2.begin(), cards2.end());
	auto pastRounds1 = PastRounds();
	auto pastRounds2 = PastRounds();
	auto rule = RecursiveRule();

	while (game.state == GameState::Ongoing)
	{
		rule.RunGame(game, pastRounds1, pastRounds2);
	}

	auto num = std::max(game.deck1.cards.size(), game.deck2.cards.size());
	auto& winnerDeck = game.state == GameState::Player1Won ? game.deck1 : game.deck2;
	auto sum = std::accumulate(winnerDeck.cards.cbegin(), winnerDeck.cards.cend(), 0, [&num](auto partialSum, const auto& card)
	{
		return partialSum + card * num--;
	});

	std::cout << "Total sum is: " << sum << std::endl;
}