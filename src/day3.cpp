#include "registry.h"
#include "maths.h"
#include "parse.h"

#include <string>
#include <iostream>
#include <map>

static bool is_parts_symbol(char c)
{
	return c != '.' && !std::isdigit(c);
}

static std::string day3_1(std::istream& i)
{
	int result = 0;

	std::string previous_line;
	std::string current_line;
	std::string next_line;

	if (!std::getline(i, current_line))
		throw std::invalid_argument("Invalid input: no data");

	auto process_current_line = [&]
	{
		for(std::size_t current_index = 0; current_index < current_line.size(); ++current_index)
		{
			char c = current_line[current_index];
			if (std::isdigit(c))
			{
				std::size_t const digit_start = current_index;
				std::size_t digit_end = current_index + 1;
				for (; digit_end < current_line.size() && std::isdigit(current_line[digit_end])
					; ++digit_end, ++current_index);

				// Check for symbol in proximity
				bool symbol_detected = false;

				if (!previous_line.empty())
				{
					std::size_t check_end = maths::min_value(previous_line.size(), digit_end + 1);
					for (std::size_t check_index = maths::max_value(digit_start, 1) - 1; check_index < check_end; ++check_index)
					{
						if (is_parts_symbol(previous_line[check_index]))
						{
							symbol_detected = true;
							break;
						}
					}
				}

				// Current line
				if (!symbol_detected)
				{
					if ((digit_start != 0 && is_parts_symbol(current_line[digit_start - 1]))
						|| (digit_end != current_line.size() && is_parts_symbol(current_line[digit_end])))
						symbol_detected = true;
				}

				if (!symbol_detected && !next_line.empty())
				{
					std::size_t check_end = maths::min_value(next_line.size(), digit_end + 1);
					for (std::size_t check_index = maths::max_value(digit_start, 1) - 1; check_index < check_end; ++check_index)
					{
						if (is_parts_symbol(next_line[check_index]))
						{
							symbol_detected = true;
							break;
						}
					}
				}
				
				if (symbol_detected)
				{
					result += to_int(std::string_view(current_line).substr(digit_start, digit_end - digit_start));
				}
			}
		}
	};

	if (!std::getline(i, next_line))
		throw std::invalid_argument("Give it two lines, come on");

	do
	{
		process_current_line();

		previous_line = std::move(current_line);
		current_line = std::move(next_line);
		next_line = "";
	} while (std::getline(i, next_line));

	process_current_line();

	return std::to_string(result);
}

static std::string day3_2(std::istream& i)
{	
	std::size_t line_number = 0;

	int result = 0;

	std::string previous_line;
	std::string current_line;
	std::string next_line;

	++line_number;
	if (!std::getline(i, current_line))
		throw std::invalid_argument("Invalid input: no data");

	std::map<std::pair<std::size_t, std::size_t>, int> gear_first_ratio;

	auto process_current_line = [&]
	{
		for(std::size_t current_index = 0; current_index < current_line.size(); ++current_index)
		{
			char c = current_line[current_index];
			if (std::isdigit(c))
			{
				std::size_t const digit_start = current_index;
				std::size_t digit_end = current_index + 1;
				for (; digit_end < current_line.size() && std::isdigit(current_line[digit_end])
					; ++digit_end, ++current_index);

				// Check for gear in proximity
				bool gear_detected = false;
				std::pair<std::size_t, std::size_t> gear_coordinate;

				if (!previous_line.empty())
				{
					std::size_t check_end = maths::min_value(previous_line.size(), digit_end + 1);
					for (std::size_t check_index = maths::max_value(digit_start, 1) - 1; check_index < check_end; ++check_index)
					{
						if (previous_line[check_index] == '*')
						{
							gear_coordinate = { check_index, line_number - 1 };
							gear_detected = true;
							break;
						}
					}
				}

				// Current line
				if (!gear_detected)
				{
					if (digit_start != 0 && current_line[digit_start - 1] == '*')
					{
						gear_coordinate = { digit_start - 1, line_number };
						gear_detected = true;
					}
					else if (digit_end != current_line.size() && current_line[digit_end] == '*')
					{
						gear_coordinate = { digit_end, line_number };
						gear_detected = true;
					}
				}

				if (!gear_detected && !next_line.empty())
				{
					std::size_t check_end = maths::min_value(next_line.size(), digit_end + 1);
					for (std::size_t check_index = maths::max_value(digit_start, 1) - 1; check_index < check_end; ++check_index)
					{
						if (next_line[check_index] == '*')
						{
							gear_coordinate = { check_index, line_number + 1 };
							gear_detected = true;
							break;
						}
					}
				}
				
				if (gear_detected)
				{
					int const number = to_int(std::string_view(current_line).substr(digit_start, digit_end - digit_start));

					auto first_ratio = gear_first_ratio.find(gear_coordinate);
					if (first_ratio != gear_first_ratio.end())
					{
						result += first_ratio->second * number;
					}
					else
					{
						gear_first_ratio.emplace(gear_coordinate, number);
					}
				}
			}
		}
	};

	if (!std::getline(i, next_line))
		throw std::invalid_argument("Give it two lines, come on");

	do
	{
		process_current_line();

		previous_line = std::move(current_line);
		current_line = std::move(next_line);
		++line_number;
		next_line = "";
	} while (std::getline(i, next_line));

	process_current_line();

	return std::to_string(result);
}

solver_registery day3(3, &day3_1, &day3_2);
