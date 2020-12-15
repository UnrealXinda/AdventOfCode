#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <bitset>
#include <assert.h>
#include "Day14.h"

namespace
{
	enum class MaskValue
	{
		Floating,
		Zero,
		One,
	};

	template <unsigned int Bits>
	struct Mask
	{		
		Mask();
		void SetMask(const std::string& str);
		std::vector<MaskValue> values;
	};

	template <class AddrType, class ValueType>
	struct Memory
	{
		struct Entry
		{
			AddrType addr;
			ValueType value;
		};

		ValueType& operator[](AddrType i);
		const ValueType& operator[](AddrType i) const;

		inline auto cbegin() const { return entries.cbegin(); }
		inline auto cend() const { return entries.cend(); }

		std::vector<Entry> entries;
	};

	template <class AddrType, class ValueType>
	struct MemoryMap
	{
		ValueType& operator[](AddrType i) { return map[i]; }
		const ValueType& operator[](AddrType i) const { return map[i]; }

		inline auto cbegin() const { return map.cbegin(); }
		inline auto cend() const { return map.cend(); }

		std::unordered_map<AddrType, ValueType> map;
	};

	template <unsigned int Bits>
	Mask<Bits>::Mask() :
		values(Bits)
	{		
	}

	template <unsigned int Bits>
	void Mask<Bits>::SetMask(const std::string& str)
	{
		assert(str.length() >= Bits);

		for (auto i = 0; i < Bits; ++i)
		{
			switch (str[i])
			{
			case 'X':
				values[Bits - i - 1] = MaskValue::Floating;
				break;
			case '0':
				values[Bits - i - 1] = MaskValue::Zero;
				break;
			case '1':
				values[Bits - i - 1] = MaskValue::One;
				break;
			}
		}
	}

	template <class AddrType, class ValueType>
	ValueType& Memory<AddrType, ValueType>::operator[](AddrType i)
	{
		auto it = std::find_if(entries.begin(), entries.end(), [i](const auto& entry) -> bool
		{
			return entry.addr == i;
		});

		if (it != entries.end())
		{
			return it->value;
		}

		auto entry = Entry{ i, static_cast<ValueType>(0) };
		entries.push_back(std::move(entry));

		return std::prev(entries.end())->value;
	}

	template <class AddrType, class ValueType>
	const ValueType& Memory<AddrType, ValueType>::operator[](AddrType i) const
	{
		auto it = std::find_if(entries.begin(), entries.end(), [i](const auto& entry) -> bool
		{
			return entry.addr == i;
		});

		if (it != entries.end())
		{
			return it->value;
		}

		auto entry = Entry{ i, static_cast<ValueType>(0) };
		entries.push_back(std::move(entry));

		return std::prev(entries.end())->value;
	}

	auto MaskBitAdd(MaskValue value, bool bit) -> bool
	{
		switch (value)
		{
		case MaskValue::Zero:
			return false;
		case MaskValue::One:
			return true;
		case MaskValue::Floating:
		default:
			return bit;
		}
	}

	template <unsigned int Bits>
	auto MaskAddBits(const Mask<Bits>& mask, const std::bitset<Bits>& bits) -> std::bitset<Bits>
	{
		auto result = std::bitset<Bits>();
		for (auto i = 0; i < Bits; ++i)
		{
			auto maskValue = mask.values[i];
			auto bit = bits[i];
			result.set(i, MaskBitAdd(maskValue, bit));
		}

		return result;
	}

	template <unsigned int Bits>
	auto MaskAdd(const Mask<Bits>& mask, const std::bitset<Bits>& bits) -> Mask<Bits>
	{
		auto result = Mask<Bits>();
		for (int i = 0; i < Bits; ++i)
		{
			switch (mask.values[i])
			{
			case MaskValue::Floating:
				result.values[i] = MaskValue::Floating;
				break;
			case MaskValue::Zero:
				result.values[i] = bits[i] ? MaskValue::One : MaskValue::Zero;
				break;
			case MaskValue::One:
				result.values[i] = MaskValue::One;
				break;
			}
		}

		return result;
	}

	template <unsigned int Bits, class AddrType>
	auto FillFloatingMaskBits(AddrType floatingBits, unsigned int floatingBitsCount, const Mask<Bits>& mask) -> AddrType
	{
		auto result = static_cast<AddrType>(0);
		for (auto i = static_cast<int>(Bits - 1); i >= 0; --i)
		{
			result <<= 1;

			switch (mask.values[i])
			{
			case MaskValue::Floating:
				result |= (floatingBits & 0x01);
				floatingBits >>= 1;
				break;
			case MaskValue::One:
				result |= 0x01;
				break;
			}
		}

		return result;
	}

	template <unsigned int Bits, class AddrType, class ValueType>
	void WriteToMemory(const Mask<Bits>& mask, MemoryMap<AddrType, ValueType>& map, AddrType addr, ValueType value)
	{
		auto floatingBitsCount = std::count(mask.values.cbegin(), mask.values.cend(), MaskValue::Floating);
		auto addrBits = std::bitset<Bits>(addr);
		auto addrMask = MaskAdd(mask, addrBits);

		for (auto i = 0; i < (1 << floatingBitsCount); ++i)
		{
			auto realAddr = FillFloatingMaskBits<Bits, AddrType>(i, floatingBitsCount, addrMask);
			map[realAddr] = value;
		}
	}
}

void Day14::SolutionPartOne(const std::string& inputFilePath)
{
	using AddrType = unsigned int;
	using ValueType = unsigned long long;
	const int kBits = 36;
	const auto kMaskRegex = std::regex("^mask = ([01X]{" + std::to_string(kBits) + "})$");
	const auto kMemRegex = std::regex(R"rgx(^mem\[(\d+)\] = (\d+)$)rgx");

	auto input = std::ifstream{ inputFilePath };
	auto memory = Memory<AddrType, ValueType>();
	auto mask = Mask<kBits>();
	auto match = std::smatch();

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, kMaskRegex))
		{
			mask.SetMask(match[1].str());
		}

		else if (std::regex_match(line, match, kMemRegex))
		{
			auto addr = std::stoi(match[1].str());
			auto value = std::stoi(match[2].str());
			auto bits = std::bitset<kBits>(value);
			memory[addr] = MaskAddBits(mask, bits).to_ullong();
		}
	}

	auto sum = std::accumulate(memory.cbegin(), memory.cend(), static_cast<ValueType>(0), [](const auto partialSum, const auto& entry) -> ValueType
	{
		return partialSum + entry.value;
	});

	std::cout << "Total sum is: " << sum << std::endl;
}

void Day14::SolutionPartTwo(const std::string& inputFilePath)
{
	using AddrType = unsigned long long;
	using ValueType = unsigned long long;
	const int kBits = 36;
	const auto kMaskRegex = std::regex("^mask = ([01X]{" + std::to_string(kBits) + "})$");
	const auto kMemRegex = std::regex(R"rgx(^mem\[(\d+)\] = (\d+)$)rgx");

	auto input = std::ifstream{ inputFilePath };
	auto memory = MemoryMap<AddrType, ValueType>();
	auto mask = Mask<kBits>();
	auto match = std::smatch();

	for (auto line = std::string(); std::getline(input, line); )
	{
		if (std::regex_match(line, match, kMaskRegex))
		{
			mask.SetMask(match[1].str());
		}

		else if (std::regex_match(line, match, kMemRegex))
		{
			auto addr = std::stoi(match[1].str());
			auto value = std::stoi(match[2].str());

			WriteToMemory<kBits, AddrType, ValueType>(mask, memory, addr, value);
		}
	}

	auto sum = std::accumulate(memory.cbegin(), memory.cend(), static_cast<ValueType>(0), [](const auto partialSum, const auto& pair) -> ValueType
	{
		return partialSum + pair.second;
	});

	std::cout << "Total sum is: " << sum << std::endl;
}

