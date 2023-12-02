#include "registry.h"

#include <map>

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

solver_registery::solver_registery(int day, part_solver* part1, part_solver* part2)
{
	get_registry().try_emplace(day, part1, part2);
}
