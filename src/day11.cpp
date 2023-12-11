#include "registry.h"

#include <iostream>
#include <format>
#include <vector>
#include <set>

namespace
{
	struct coord_t
	{
		std::size_t x;
		std::size_t y;

		auto operator<=>(coord_t const&) const = default;
	};
}

static std::string day11_1(std::istream& i)
{
	std::size_t line_number = 0;

	std::size_t line_size = 0;
	std::vector<coord_t> galaxies;
	std::set<std::size_t> columns;
	std::set<std::size_t> rows;
	while (i && !i.eof())
	{
		std::size_t char_number = 0;
		while (i && i.peek() != '\n')
		{
			int n = i.get();
			if (i.eof())
				break;

			if (i.bad() || n == -1)
				throw std::invalid_argument(std::format("Bad character on line {}", line_number + 1));

			char c = static_cast<char>(n);
			if (c == '#')
			{
				galaxies.emplace_back(char_number, line_number);
				columns.emplace(char_number);
				rows.emplace(line_number);
			}

			++char_number;
		}

		if (line_size == 0)
			line_size = char_number;
		else if (line_size != char_number)
			throw std::invalid_argument(std::format("Inconsistent character count on line {}", line_number + 1));

		i.ignore(1);
		++line_number;
		char_number = 0;
	}

	std::size_t const line_count = line_number;

	std::set<std::size_t> free_columns;
	for (std::size_t i = 0; i < line_size; ++i)
	{
		if (!columns.contains(i))
			free_columns.emplace(i);
	}

	std::set<std::size_t> free_rows;
	for (std::size_t j = 0; j < line_count; ++j)
	{
		if (!rows.contains(j))
			free_rows.emplace(j);
	}

	// Computer
	std::size_t result = 0;
	for (std::size_t i = 0; i < galaxies.size() - 1; ++i)
	{
		for (std::size_t j = i + 1; j < galaxies.size(); ++j)
		{
			std::size_t current_distance = 0;

			coord_t const& g1 = galaxies[i];
			coord_t const& g2 = galaxies[j];

			coord_t const& left_g = std::ranges::min(g1, g2, std::less{}, &coord_t::x);
			coord_t const& right_g = std::ranges::max(g2, g1, std::less{}, &coord_t::x);
			coord_t const& top_g = std::ranges::min(g1, g2, std::less{}, &coord_t::y);
			coord_t const& bottom_g = std::ranges::max(g2, g1, std::less{}, &coord_t::y);

			for (std::size_t x = left_g.x; x < right_g.x; ++x)
			{
				if (free_columns.contains(x))
					current_distance += 2;
				else
					current_distance += 1;
			}

			for (std::size_t y = top_g.y; y < bottom_g.y; ++y)
			{
				if (free_rows.contains(y))
					current_distance += 2;
				else
					current_distance += 1;
			}

			result += current_distance;
		}
	}

	return std::to_string(result);
}

static std::string day11_2(std::istream& i)
{
	std::size_t line_number = 0;

	std::size_t line_size = 0;
	std::vector<coord_t> galaxies;
	std::set<std::size_t> columns;
	std::set<std::size_t> rows;
	while (i && !i.eof())
	{
		std::size_t char_number = 0;
		while (i && i.peek() != '\n')
		{
			int n = i.get();
			if (i.eof())
				break;

			if (i.bad() || n == -1)
				throw std::invalid_argument(std::format("Bad character on line {}", line_number + 1));

			char c = static_cast<char>(n);
			if (c == '#')
			{
				galaxies.emplace_back(char_number, line_number);
				columns.emplace(char_number);
				rows.emplace(line_number);
			}

			++char_number;
		}

		if (line_size == 0)
			line_size = char_number;
		else if (line_size != char_number)
			throw std::invalid_argument(std::format("Inconsistent character count on line {}", line_number + 1));

		i.ignore(1);
		++line_number;
		char_number = 0;
	}

	std::size_t const line_count = line_number;

	std::set<std::size_t> free_columns;
	for (std::size_t i = 0; i < line_size; ++i)
	{
		if (!columns.contains(i))
			free_columns.emplace(i);
	}

	std::set<std::size_t> free_rows;
	for (std::size_t j = 0; j < line_count; ++j)
	{
		if (!rows.contains(j))
			free_rows.emplace(j);
	}

	// Computer
	std::size_t result = 0;
	for (std::size_t i = 0; i < galaxies.size() - 1; ++i)
	{
		for (std::size_t j = i + 1; j < galaxies.size(); ++j)
		{
			std::size_t current_distance = 0;

			coord_t const& g1 = galaxies[i];
			coord_t const& g2 = galaxies[j];

			coord_t const& left_g = std::ranges::min(g1, g2, std::less{}, &coord_t::x);
			coord_t const& right_g = std::ranges::max(g2, g1, std::less{}, &coord_t::x);
			coord_t const& top_g = std::ranges::min(g1, g2, std::less{}, &coord_t::y);
			coord_t const& bottom_g = std::ranges::max(g2, g1, std::less{}, &coord_t::y);

			for (std::size_t x = left_g.x; x < right_g.x; ++x)
			{
				if (free_columns.contains(x))
					current_distance += 1'000'000;
				else
					current_distance += 1;
			}

			for (std::size_t y = top_g.y; y < bottom_g.y; ++y)
			{
				if (free_rows.contains(y))
					current_distance += 1'000'000;
				else
					current_distance += 1;
			}

			result += current_distance;
		}
	}

	return std::to_string(result);
}

solver_registery day11(11, &day11_1, &day11_2);
