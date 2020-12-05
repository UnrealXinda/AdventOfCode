#include <iostream>
#include "Day1.h"
#include "Day2.h"
#include "Day3.h"
#include "Day4.h"

int main()
{
	std::cout << "***********" << std::endl;
	std::cout << "Day 1 Part One: " << std::endl;
	Day1::SolutionPartOne("Inputs/Day1.txt", 2020);
	std::cout << "-----------" << std::endl;

	std::cout << "Day 1 Part Two: " << std::endl;
	Day1::SolutionPartTwo("Inputs/Day1.txt", 2020);

	std::cout << "***********" << std::endl;
	std::cout << "Day 2 Part One: " << std::endl;
	Day2::SolutionPartOne("Inputs/Day2.txt");
	std::cout << "-----------" << std::endl;

	std::cout << "Day 2 Part Two: " << std::endl;
	Day2::SolutionPartTwo("Inputs/Day2.txt");

	std::cout << "***********" << std::endl;
	std::cout << "Day 3 Part One: " << std::endl;
	Day3::SolutionPartOne("Inputs/Day3.txt");

	std::cout << "-----------" << std::endl;
	std::cout << "Day 3 Part Two: " << std::endl;
	Day3::SolutionPartTwo("Inputs/Day3.txt");

	std::cout << "***********" << std::endl;
	std::cout << "Day 4 Part One: " << std::endl;
	Day4::SolutionPartOne("Inputs/Day4.txt");

	std::cout << "-----------" << std::endl;
	std::cout << "Day 4 Part Two: " << std::endl;
	Day4::SolutionPartTwo("Inputs/Day4.txt");
}