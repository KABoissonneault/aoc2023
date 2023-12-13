#pragma once

#include <string>
#include <iosfwd>

using part_solver = void(std::istream&, std::ostream&);
using legacy_part_solver = std::string(std::istream&);

struct day_solver
{
	part_solver* part1;
	part_solver* part2;
	legacy_part_solver* part1_legacy;
	legacy_part_solver* part2_legacy;

	void solve_part1(std::istream& i, std::ostream& o);
	void solve_part2(std::istream& i, std::ostream& o);
};

bool has_solver(int day);
day_solver get_solver(int day);

class solver_registery
{
public:
	solver_registery(int day, part_solver* part1, part_solver* part2);
	solver_registery(int day, legacy_part_solver* part1, legacy_part_solver* part2);
};
