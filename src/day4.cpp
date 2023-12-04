#include "registry.h"

#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <deque>

static std::string day4_1(std::istream& i)
{
	int line_number = 0;

	int result = 0;

	do
	{
		++line_number;

		i.ignore(std::numeric_limits<std::streamsize>::max(), ':');
		if (i.eof())
			break;

		std::vector<int> const winning_numbers = std::vector<int>(std::istream_iterator<int>(i), std::istream_iterator<int>());
		if (i.eof() || i.bad())
			throw std::invalid_argument(std::format("Could not parse winning numbers on line {}", line_number));

		i.clear();
		i.ignore(std::numeric_limits<std::streamsize>::max(), '|');

		int winning_count = 0;
		int current_number;
		while (i.peek() != '\n' && i >> current_number)
		{
			if (std::ranges::find(winning_numbers, current_number) != winning_numbers.end())
			{
				++winning_count;
			}
		}

		if (!i.eof() && i.fail())
			throw std::invalid_argument(std::format("Error parsing current numbers on line {}", line_number));

		if (winning_count == 0)
			continue;

		result += 1 << (winning_count - 1);
	} while (i);

	return std::to_string(result);
}

static std::string day4_2(std::istream& i)
{
	int line_number = 0;

	int result = 0;

	std::deque<int> copy_count;

	do
	{
		++line_number;

		i.ignore(std::numeric_limits<std::streamsize>::max(), ':');
		if (i.eof())
			break;

		std::vector<int> const winning_numbers = std::vector<int>(std::istream_iterator<int>(i), std::istream_iterator<int>());
		if (i.eof() || i.bad())
			throw std::invalid_argument(std::format("Could not parse winning numbers on line {}", line_number));

		i.clear();
		i.ignore(std::numeric_limits<std::streamsize>::max(), '|');

		int winning_count = 0;
		int current_number;
		while (i.peek() != '\n' && i >> current_number)
		{
			if (std::ranges::find(winning_numbers, current_number) != winning_numbers.end())
			{
				++winning_count;
			}
		}

		if (!i.eof() && i.fail())
			throw std::invalid_argument(std::format("Error parsing current numbers on line {}", line_number));

		int card_count;
		if (copy_count.empty())
		{
			card_count = 1;
		}
		else
		{
			card_count = copy_count.front() + 1;
			copy_count.pop_front();
		}

		result += card_count;

		if (winning_count != 0)
		{
			for (int i = 0; i < winning_count; ++i)
			{
				if (i < copy_count.size())
					copy_count[i] += card_count;
				else
					copy_count.push_back(card_count);
			}
		}
	} while (i);

	return std::to_string(result);
}

solver_registery day4(4, &day4_1, &day4_2);
