#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include "Day18.h"

namespace
{
	enum class Operator
	{
		Invalid,
		Add,
		Sub,
		Mul,
		Div,
		LeftParenthesis,
		RightParenthesis,
	};

	using OpPriorityMap = std::unordered_map<Operator, unsigned int>;

	inline auto OpFromChar(char ch) -> Operator
	{
		switch (ch)
		{
		case '+':
			return Operator::Add;
		case '-':
			return Operator::Sub;
		case '*':
			return Operator::Mul;
		case '/':
			return Operator::Div;
		case '(':
			return Operator::LeftParenthesis;
		case ')':
			return Operator::RightParenthesis;
		default:
			return Operator::Invalid;
		}
	}

	inline auto CharFromOp(Operator op) -> char
	{
		switch (op)
		{
		case Operator::Add:
			return '+';
		case Operator::Sub:
			return '-';
		case Operator::Mul:
			return '*';
		case Operator::Div:
			return '/';
		case Operator::LeftParenthesis:
			return '(';
		case Operator::RightParenthesis:
			return ')';
		case Operator::Invalid:
			return ' ';
		}
	}

	template <class T>
	auto RunOp(Operator op, T a, T b) -> T
	{
		switch (op)
		{
		case Operator::Add:
			return a + b;
		case Operator::Sub:
			return a - b;
		case Operator::Mul:
			return a * b;
		case Operator::Div:
			return a / b;
		case Operator::LeftParenthesis:
		case Operator::RightParenthesis:
		case Operator::Invalid:
			assert(false);
			return 0;
		}
	}

	template <class T>
	auto InfixToPostfix(const std::string& infix, const OpPriorityMap& opPriorities) -> std::string
	{
		auto postfix = std::stringstream();
		auto opStack = std::stack<Operator>();

		for (auto ch : infix)
		{
			if (std::isdigit(ch))
			{
				postfix << ch;
			}
			else if (ch == '(')
			{
				opStack.push(Operator::LeftParenthesis);
			}
			else if (ch == ')')
			{
				auto topOp = opStack.top();
				opStack.pop();

				while (topOp != Operator::LeftParenthesis)
				{
					postfix << CharFromOp(topOp);
					topOp = opStack.top();
					opStack.pop();
				}
			}
			else
			{
				auto op = OpFromChar(ch);
				assert(op != Operator::Invalid);

				while (!opStack.empty() && opPriorities.at(opStack.top()) >= opPriorities.at(op))
				{
					postfix << CharFromOp(opStack.top());
					opStack.pop();
				}
				opStack.push(op);
			}
		}

		while (!opStack.empty())
		{
			postfix << CharFromOp(opStack.top());
			opStack.pop();
		}

		return postfix.str();
	}

	template <class T>
	auto EvaluatePostfixExpression(const std::string& postfix) -> T
	{
		auto operands = std::stack<T>();

		for (auto ch : postfix)
		{
			if (std::isdigit(ch))
			{
				operands.push(ch - '0');
			}
			else
			{
				auto op1 = operands.top(); operands.pop();
				auto op2 = operands.top(); operands.pop();
				auto op = OpFromChar(ch);

				operands.push(RunOp(op, op1, op2));
			}
		}

		return operands.top();
	}

	template <class T>
	auto EvaluateArithmaticExpression(std::string expr, const OpPriorityMap& opPriorities) -> T
	{
		expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
		auto postfix = InfixToPostfix<T>(expr, opPriorities);
		return EvaluatePostfixExpression<T>(postfix);
	}
}

void Day18::SolutionPartOne(const std::string& inputFilePath)
{
	const auto priorityMap = OpPriorityMap
	{
		{ Operator::Add, 1 },
		{ Operator::Sub, 1 },
		{ Operator::Mul, 1 },
		{ Operator::Div, 1 },
		{ Operator::LeftParenthesis, 0 },
		{ Operator::RightParenthesis, 0 },
	};
	auto input = std::ifstream(inputFilePath);
	auto sum = 0LL;

	for (auto line = std::string(); std::getline(input, line); )
	{
		sum += EvaluateArithmaticExpression<long long>(line, priorityMap);
	}
	std::cout << "Total sum: " << sum << std::endl;
}

void Day18::SolutionPartTwo(const std::string& inputFilePath)
{
	const auto priorityMap = OpPriorityMap
	{
		{ Operator::Add, 2 },
		{ Operator::Sub, 2 },
		{ Operator::Mul, 1 },
		{ Operator::Div, 1 },
		{ Operator::LeftParenthesis, 0 },
		{ Operator::RightParenthesis, 0 },
	};
	auto input = std::ifstream(inputFilePath);
	auto sum = 0LL;

	for (auto line = std::string(); std::getline(input, line); )
	{
		sum += EvaluateArithmaticExpression<long long>(line, priorityMap);
	}
	std::cout << "Total sum: " << sum << std::endl;
}