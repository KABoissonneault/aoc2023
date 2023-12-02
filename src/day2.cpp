#include "parse.h"
#include "registry.h"

#include <string_view>
#include <string>
#include <iostream>
#include <format>

template<typename DrawCallback>
static void parse_play(std::string_view line, int line_number, const DrawCallback& callback)
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

solver_registery day2(2, &day2_1, &day2_2);
