#pragma once

#include <string>
#include <iosfwd>

using part_solver = std::string(std::istream&);

struct day_solver
{
	part_solver* part1;
	part_solver* part2;
};

bool has_solver(int day);
day_solver get_solver(int day);

class solver_registery
{
public:
	solver_registery(int day, part_solver* part1, part_solver* part2);
};
