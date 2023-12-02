#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <format>
#include <fstream>

namespace
{
	std::string day1_1(std::istream& i)
	{
		std::string line;
		int line_number = 0;

		int result = 0;

		while (std::getline(i, line)) 
		{
			++line_number;

			int number = 0;
			std::size_t last_index = -1;
			for (std::size_t index = 0; index < line.size(); ++index) 
			{
				char c = line[index];
				if (std::isdigit(c)) 
				{
					int digit = c - '0';
					if (number < 10)
						number = digit * 10;

					last_index = index;
				}
			}

			if (last_index == -1) 
				throw std::invalid_argument(std::format("Input line {} was invalid", line_number));

			char c = line[last_index];
			number += c - '0';

			result += number;
		}

		return std::to_string(result);
	}

	char const* digit_strings[] =
	{
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
	};

	std::string day1_2(std::istream& i)
	{
		std::string line;
		int line_number = 0;

		int result = 0;

		while (std::getline(i, line))
		{
			std::string_view const line_view(line);
			++line_number;

			int number = 0;
			int last_digit = -1;
			for (std::size_t index = 0; index < line.size(); ++index)
			{
				char const c = line[index];
				if (std::isdigit(c))
				{
					int digit = c - '0';
					if (number < 10)
						number = digit * 10;

					last_digit = digit;
				}
				else
				{
					std::string_view const line_remainder = line_view.substr(index);
					for(int digit = 0; digit < std::size(digit_strings); ++digit)
					{
						if (line_remainder.starts_with(digit_strings[static_cast<size_t>(digit)]))
						{
							if (number < 10)
								number = digit * 10;
							last_digit = digit;
						}
					}
				}
			}

			if (last_digit == -1)
				throw std::invalid_argument(std::format("Input line {} was invalid", line_number));

			number += last_digit;

			result += number;
		}

		return std::to_string(result);
	}

	using puzzle_solver = std::string(std::istream&);

	struct day_solver
	{
		puzzle_solver* part1;
		puzzle_solver* part2;
	};

	day_solver solvers[] =
	{
		{&day1_1, &day1_2}
	};
}

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

	if (day > std::size(solvers)) 
	{
		std::printf("Day %d not implemented\n", day);
		return -1;
	}

	day_solver solver = solvers[day - 1];

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
