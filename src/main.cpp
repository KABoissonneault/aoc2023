#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <format>
#include <fstream>

namespace
{
	int to_int(std::string_view s)
	{
		int value;

		if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc())
			return value;

		throw std::invalid_argument(std::format("String '{}' was not a number", s));
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

	template<typename DrawCallback>
	void parse_play(std::string_view line, int line_number, const DrawCallback& callback)
	{			
		bool stop = false;
		auto parse_play = [&](std::string_view play) -> void
		{
			auto parse_draw = [&](std::string_view draw)
			{
				size_t count_end = draw.find_first_of(' ');
				if (count_end == std::string_view::npos)
					throw std::invalid_argument(std::format("Draw '{}' on line {} did not start with a valid number", draw, line_number));

				int const count = to_int(draw.substr(0, count_end));
				draw.remove_prefix(count_end + 1);

				stop = callback(count, draw);
			};

			for(size_t draw_end = play.find_first_of(','); draw_end != std::string_view::npos; draw_end = play.find_first_of(','))
			{
				parse_draw(play.substr(0, draw_end));

				if (stop)
					return;

				play.remove_prefix(draw_end + 2);
			}

			parse_draw(play);
		};

		for (size_t play_end = line.find_first_of(';'); play_end != std::string_view::npos; play_end = line.find_first_of(';'))
		{
			parse_play(line.substr(0, play_end));

			if (stop)
				return;

			line.remove_prefix(play_end + 2);
		}

		parse_play(line);
	}

	std::string day2_1(std::istream& i)
	{
		std::string line;
		int line_number = 0;

		int result = 0;

		while (std::getline(i, line))
		{
			++line_number;

			std::string_view line_remainder(line);
			if (!line_remainder.starts_with("Game "))
				throw std::invalid_argument(std::format("Line {} does not start with 'Game'", line_number));

			line_remainder.remove_prefix(5);

			std::size_t token_end = line_remainder.find_first_of(':');
			if (token_end == std::string_view::npos)
				throw std::invalid_argument(std::format("Line {} does not have a game id", line_number));

			int const game_id = to_int(line_remainder.substr(0, token_end));

			line_remainder.remove_prefix(token_end + 2);
						
			bool invalid = false;
			parse_play(line_remainder, line_number, [&](int count, std::string_view color)
			{
				if (color == "red")
				{
					if (count > 12)
						invalid = true;
				}
				else if (color == "green")
				{
					if (count > 13)
						invalid = true;
				}
				else if (color == "blue")
				{
					if (count > 14)
						invalid = true;
				}
				else
				{
					throw std::invalid_argument(std::format("Invalid color '{}' on line {}", color, line_number));
				}

				return invalid;
			});

			if (!invalid)
				result += game_id;
		}

		return std::to_string(result);
	}

	std::string day2_2(std::istream& i)
	{
		std::string line;
		int line_number = 0;

		int result = 0;

		while (std::getline(i, line))
		{
			++line_number;

			std::string_view line_remainder(line);
			if (!line_remainder.starts_with("Game "))
				throw std::invalid_argument(std::format("Line {} does not start with 'Game'", line_number));

			line_remainder.remove_prefix(5);

			std::size_t token_end = line_remainder.find_first_of(':');
			if (token_end == std::string_view::npos)
				throw std::invalid_argument(std::format("Line {} does not have a game id", line_number));

			int const game_id = to_int(line_remainder.substr(0, token_end));

			line_remainder.remove_prefix(token_end + 2);
						
			int max_red = 0;
			int max_green = 0;
			int max_blue = 0;
			parse_play(line_remainder, line_number, [&](int count, std::string_view color)
			{
				if (color == "red")
				{
					max_red = std::max(count, max_red);
				}
				else if (color == "green")
				{
					max_green = std::max(count, max_green);
				}
				else if (color == "blue")
				{
					max_blue = std::max(count, max_blue);
				}
				else
				{
					throw std::invalid_argument(std::format("Invalid color '{}' on line {}", color, line_number));
				}

				return false;
			});

			result += max_red * max_green * max_blue;
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
		{&day1_1, &day1_2},
		{&day2_1, &day2_2},
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
