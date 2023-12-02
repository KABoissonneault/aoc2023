#define _CRT_SECURE_NO_WARNINGS

#include "registry.h"

#include <cstdio>
#include <format>
#include <fstream>

int main(int argc, char* argv[])
{
	std::printf("Enter a day: ");
	
	int day;
	int parsed = std::scanf("%d", &day);

	std::printf("\n");

	if (parsed != 1) 
	{
		std::printf("Invalid day\n");
		return -1;
	}

	if (day <= 0)
	{
		std::printf("Day %d is invalid\n", day);
		return -1;
	}

	if (!has_solver(day)) 
	{
		std::printf("Day %d not implemented\n", day);
		return -1;
	}

	day_solver solver = get_solver(day);

	std::ifstream input(std::format("input/day{}.txt", day));
	if (!input)
	{
		std::printf("Day %d has no input\n", day);
		return -1;
	}

	std::printf("Part 1:\n");
	try 
	{
		std::string solution1 = solver.part1(input);
		std::puts(solution1.c_str());
	} 
	catch (const std::exception& e) 
	{
		std::puts(e.what());
		return -1;
	}

	input.clear();
	input.seekg(0);

	do
	{
		if (solver.part2 == nullptr)
		{
			std::printf("Part 2:\n (Not implemented)\n");
			break;
		}

		std::printf("Part 2:\n");
		try
		{
			std::string solution2 = solver.part2(input);
			std::puts(solution2.c_str());
		}
		catch (const std::exception& e)
		{
			std::puts(e.what());
			return -1;
		}
	} while (false);

	return 0;
}
