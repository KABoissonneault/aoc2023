#include "registry.h"

#include <string>
#include <iostream>
#include <format>

namespace
{
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
}

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
				for (int digit = 0; digit < std::size(digit_strings); ++digit)
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

solver_registery day1(1, &day1_1, &day1_2);
