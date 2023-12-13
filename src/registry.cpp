#include "registry.h"

#include <map>
#include <iostream>

static std::map<int, day_solver>& get_registry()
{
	static std::map<int, day_solver> solvers;
	return solvers;
}

bool has_solver(int day)
{
	return get_registry().contains(day);
}

day_solver get_solver(int day)
{
	return get_registry()[day];
}

void day_solver::solve_part1(std::istream& i, std::ostream& o)
{
	if (part1 != nullptr)
	{
		part1(i, o);
	}
	else if (part1_legacy != nullptr)
	{
		o << part1_legacy(i);
	}
	else
	{
		o << "Not implemented";
	}
}

void day_solver::solve_part2(std::istream& i, std::ostream& o)
{
	if (part2 != nullptr)
	{
		part2(i, o);
	}
	else if (part2_legacy != nullptr)
	{
		o << part2_legacy(i);
	}
	else
	{
		o << "Not implemented";
	}
}

solver_registery::solver_registery(int day, part_solver* part1, part_solver* part2)
{
	get_registry().try_emplace(day, part1, part2);
}

solver_registery::solver_registery(int day, legacy_part_solver* part1, legacy_part_solver* part2)
{
	get_registry().try_emplace(day, nullptr, nullptr, part1, part2);
}
