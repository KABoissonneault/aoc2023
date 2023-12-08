#include "registry.h"

#include <iostream>
#include <vector>
#include <map>
#include <format>
#include <algorithm>

namespace
{
	enum class direction
	{
		left,
		right,
	};

	struct node
	{
		std::string identifier;
		std::string left_destination;
		std::string right_destination;
	};

	struct path
	{
		std::string current_pos;
		std::size_t cycle_steps = 0;
		std::size_t steps = 0;
	};
}

static std::string day8_1(std::istream& i)
{
	int line_number = 1;

	std::vector<direction> instructions;
	while (i.peek() != '\n')
	{
		char c = i.get();
		if (c == 'L')
			instructions.push_back(direction::left);
		else if (c == 'R')
			instructions.push_back(direction::right);
		else
			throw std::invalid_argument("Invalid direction");
	}

	i.ignore(1);

	++line_number;

	i.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	++line_number;

	std::map<std::string, node> map;

	std::string line;
	while (std::getline(i, line))
	{
		node n;
		n.identifier.resize(3);
		n.left_destination.resize(3);
		n.right_destination.resize(3);

		int const parsed = std::sscanf(line.c_str(), "%3s = (%3s, %3s)", n.identifier.data(), n.left_destination.data(), n.right_destination.data());
		if (parsed != 3)
			throw std::invalid_argument(std::format("Invalid node on line {}", line_number));

		map.emplace(n.identifier, std::move(n));

		++line_number;
	}

	std::size_t steps = 0;
	std::size_t instruction_index = 0;

	std::string current_pos = "AAA";
	do
	{
		node const& current_node = map[current_pos];
		direction const dir = instructions[instruction_index];
		if (dir == direction::left)
		{
			current_pos = current_node.left_destination;
		}
		else
		{
			current_pos = current_node.right_destination;
		}

		++instruction_index;
		if (instruction_index == instructions.size())
			instruction_index = 0;

		++steps;
	} while (current_pos != "ZZZ");

	return std::to_string(steps);
}

static std::string day8_2(std::istream& i)
{
	int line_number = 1;

	std::vector<direction> instructions;
	while (i.peek() != '\n')
	{
		char c = i.get();
		if (c == 'L')
			instructions.push_back(direction::left);
		else if (c == 'R')
			instructions.push_back(direction::right);
		else
			throw std::invalid_argument("Invalid direction");
	}

	i.ignore(1);

	++line_number;

	i.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	++line_number;

	std::map<std::string, node> map;
	std::vector<path> paths;

	std::string line;
	while (std::getline(i, line))
	{
		node n;
		n.identifier.resize(3);
		n.left_destination.resize(3);
		n.right_destination.resize(3);

		int const parsed = std::sscanf(line.c_str(), "%3s = (%3s, %3s)", n.identifier.data(), n.left_destination.data(), n.right_destination.data());
		if (parsed != 3)
			throw std::invalid_argument(std::format("Invalid node on line {}", line_number));

		if (n.identifier.back() == 'A')
			paths.emplace_back(n.identifier);

		map.emplace(n.identifier, std::move(n));
		
		++line_number;
	}

	std::size_t steps = 0;
	std::size_t instruction_index = 0;

	do
	{
		++steps;

		for (path& p : paths)
		{
			if (p.cycle_steps != 0)
				continue;

			node const& current_node = map[p.current_pos];
			direction const dir = instructions[instruction_index];
			if (dir == direction::left)
			{
				p.current_pos = current_node.left_destination;
			}
			else
			{
				p.current_pos = current_node.right_destination;
			}

			if (p.current_pos.back() == 'Z')
				p.cycle_steps = steps;
		}

		++instruction_index;
		if (instruction_index == instructions.size())
			instruction_index = 0;
	} while (!std::ranges::all_of(paths, [](path const& p) { return p.cycle_steps != 0; }));

	for (path& p : paths)
	{
		p.steps = p.cycle_steps;
	}

	std::size_t reference_value = paths[0].steps;
	do
	{
		for (path& p : paths)
		{
			if (p.steps < reference_value)
				p.steps += p.cycle_steps;
		}

		bool const all_equal = std::ranges::all_of(paths, [reference_value](std::size_t s) { return s == reference_value; }, &path::steps);

		if (all_equal)
		{
			break;
		}

		bool const any_smaller = std::ranges::any_of(paths, [reference_value](std::size_t s) { return s < reference_value; }, &path::steps);
		if (!any_smaller)
			reference_value += paths[0].cycle_steps;

	} while (true);

	return std::to_string(reference_value);
}

solver_registery day8(8, &day8_1, &day8_2);
