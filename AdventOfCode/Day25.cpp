#include <iostream>
#include <fstream>
#include <cmath>
#include "Day25.h"

namespace
{
	template <class T>
	auto TransformNumber(T subject, T loopSize, T divisor) -> T
	{
		auto base = static_cast<T>(1);
		auto logSubject = std::log(subject);
		auto log = std::log(static_cast<long double>(divisor) / base) / std::log(subject);

		while (log < loopSize)
		{
			auto exp = static_cast<T>(std::ceil(log));
			auto pow = static_cast<T>(std::pow(subject, exp));
			base = (pow * base) % divisor;
			log = std::log(static_cast<long double>(divisor) / base) / logSubject;
			loopSize -= exp;
		}

		for (auto i = 0; i < loopSize; ++i)
		{
			base *= subject;
			base %= divisor;
		}

		return base;
	}

	template <class T>
	auto GetLoopSize(T subject, T divisor, T publicKey) -> T
	{
		auto loopSize = static_cast<T>(0);
		auto transform = 1ULL;

		while (transform != publicKey)
		{
			transform *= subject;
			transform %= divisor;
			loopSize++;
		}

		return loopSize;
	}
}

void Day25::SolutionPartOne(const std::string& inputFilePath)
{
	constexpr auto kDivisor = 20201227ULL;
	constexpr auto kStartSubject = 7ULL;
	auto input = std::ifstream(inputFilePath);
	auto line = std::string();

	std::getline(input, line);
	auto cardPublicKey = std::stoull(line);

	std::getline(input, line);
	auto doorPublicKey = std::stoull(line);

	auto cardLoopSize = GetLoopSize(kStartSubject, kDivisor, cardPublicKey);
	auto encryptionKey = TransformNumber(doorPublicKey, cardLoopSize, kDivisor);

	std::cout << "Encryption key is: " << encryptionKey;
}

void Day25::SolutionPartTwo(const std::string& inputFilePath)
{
	std::cout << "Oh there's no part 2. I've done it all!" << std::endl;
	std::cout << "Happy Holiday!" << std::endl;
}