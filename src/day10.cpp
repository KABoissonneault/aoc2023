#include "registry.h"

#include <vector>
#include <memory>
#include <iostream>
#include <format>
#include <numeric>

namespace
{
	enum class dir_t
	{
		north = 1,
		west = 2,
		south = 4,
		east = 8,
	};

	enum class piece_t
	{
		start = -1,
		ground = 0,
		bend_nw = 3,
		vert = 5,
		bend_sw = 6,
		bend_ne = 9,
		hor = 10,
		bend_se = 12,
	};

	[[nodiscard]] constexpr dir_t operator&(piece_t p, dir_t d) noexcept
	{
		return static_cast<dir_t>(static_cast<int>(p) & static_cast<int>(d));
	}

	[[nodiscard]] constexpr piece_t operator|(piece_t p, dir_t d) noexcept
	{
		return static_cast<piece_t>(static_cast<int>(p) | static_cast<int>(d));
	}

	[[nodiscard]] constexpr bool has_dir(piece_t p, dir_t d) noexcept
	{
		return (p & d) == d;
	}

	piece_t to_piece(char c)
	{
		switch (c)
		{
		case '|': return piece_t::vert;
		case '-': return piece_t::hor;
		case 'L': return piece_t::bend_ne;
		case 'J': return piece_t::bend_nw;
		case '7': return piece_t::bend_sw;
		case 'F': return piece_t::bend_se;
		case '.': return piece_t::ground;
		case 'S': return piece_t::start;
		}

		throw std::invalid_argument(std::format("Character '{}' is not a valid piece", c));
	}

	char to_char(piece_t p)
	{
		switch (p)
		{
		case piece_t::vert: return '|';
		case piece_t::hor: return '-';
		case piece_t::bend_ne: return 'L';
		case piece_t::bend_nw: return 'J';
		case piece_t::bend_sw: return '7';
		case piece_t::bend_se: return 'F';
		case piece_t::ground: return '.';
		case piece_t::start: return 'S';
		}

		throw std::invalid_argument("Not a valid piece");
	}

	struct coord_t
	{
		std::size_t x;
		std::size_t y;

		auto operator<=>(coord_t const&) const = default;
	};

	enum class status_t
	{
		unset,
		inside,
		outside,
		loop,
	};

	enum class rot_dir_t
	{
		none,
		clockwise,
		counter_clockwise,
	};

	dir_t apply_rot(dir_t d, rot_dir_t r)
	{
		if (r == rot_dir_t::none)
			return d;

		if (r == rot_dir_t::clockwise)
		{
			switch (d)
			{
			case dir_t::north: return dir_t::east;
			case dir_t::west: return dir_t::north;
			case dir_t::south: return dir_t::west;
			case dir_t::east: return dir_t::south;
			}
		}
		else
		{
			switch (d)
			{
			case dir_t::north: return dir_t::west;
			case dir_t::west: return dir_t::south;
			case dir_t::south: return dir_t::east;
			case dir_t::east: return dir_t::north;
			}
		}

		throw std::invalid_argument("Invalid direction");
	}

	rot_dir_t get_rotation(piece_t piece, dir_t provenance)
	{
		switch (piece)
		{
		case piece_t::bend_nw:
			if (!(provenance == dir_t::north || provenance == dir_t::west))
				throw std::invalid_argument("Wrong provenance for direction");
			return provenance == dir_t::north ? rot_dir_t::clockwise : rot_dir_t::counter_clockwise;
		case piece_t::bend_sw:
			if (!(provenance == dir_t::south || provenance == dir_t::west))
				throw std::invalid_argument("Wrong provenance for direction");
			return provenance == dir_t::west ? rot_dir_t::clockwise : rot_dir_t::counter_clockwise;

		case piece_t::bend_se:
			if (!(provenance == dir_t::south || provenance == dir_t::east))
				throw std::invalid_argument("Wrong provenance for direction");
			return provenance == dir_t::south ? rot_dir_t::clockwise : rot_dir_t::counter_clockwise;
		case piece_t::bend_ne:
			if (!(provenance == dir_t::north || provenance == dir_t::east))
				throw std::invalid_argument("Wrong provenance for direction");
			return provenance == dir_t::east ? rot_dir_t::clockwise : rot_dir_t::counter_clockwise;
		}

		return rot_dir_t::none;
	}

	dir_t get_other_dir(piece_t p, dir_t d)
	{
		switch (p)
		{
		case piece_t::bend_ne:
		case piece_t::bend_sw:
			switch (d)
			{
			case dir_t::north: return dir_t::east;
			case dir_t::west: return dir_t::south;
			case dir_t::south: return dir_t::west;
			case dir_t::east: return dir_t::north;
			}
			throw std::invalid_argument("Invalid dir");

		case piece_t::bend_nw:
		case piece_t::bend_se:
			switch (d)
			{
			case dir_t::north: return dir_t::west;
			case dir_t::west: return dir_t::north;
			case dir_t::south: return dir_t::east;
			case dir_t::east: return dir_t::south;
			}
			throw std::invalid_argument("Invalid dir");
		}

		return dir_t{};
	}
}

static std::string day10_1(std::istream& i)
{
	std::size_t line_number = 1;

	coord_t start_coord = {};
	std::vector<std::vector<piece_t>> piece_grid;
	
	std::size_t line_size = 0;

	do
	{
		std::vector<piece_t> piece_line;
		do
		{			
			int c = i.get();
			if (i.eof())
			{
				if (piece_line.size() != line_size)
					throw std::invalid_argument(std::format("Invalid input on line {}", line_number));

				break;
			}

			if (i.bad() || c == -1)
				throw std::invalid_argument(std::format("Invalid input on line {}", line_number));

			if (line_size != 0 && piece_line.size() == line_size)
				throw std::invalid_argument(std::format("Invalid input on line {}", line_number));


			piece_t const p = to_piece(static_cast<char>(c));
			if (p == piece_t::start)
				start_coord = { piece_line.size(), line_number - 1 };

			piece_line.push_back(p);
		} while (i && i.peek() != '\n');

		if (line_size == 0)
			line_size = piece_line.size();

		piece_grid.push_back(std::move(piece_line));
		piece_line.clear();

		i.ignore(1);
		++line_number;
	} while (i);

	piece_t start_piece = piece_t::ground;
	if (start_coord.x != 0 && has_dir(piece_grid[start_coord.y][start_coord.x - 1], dir_t::east))
		start_piece = start_piece | dir_t::west;

	if (start_coord.x != line_size - 1 && has_dir(piece_grid[start_coord.y][start_coord.x + 1], dir_t::west))
		start_piece = start_piece | dir_t::east;

	if (start_coord.y != 0 && has_dir(piece_grid[start_coord.y - 1][start_coord.x], dir_t::south))
		start_piece = start_piece | dir_t::north;

	if (start_coord.y != piece_grid.size() - 1 && has_dir(piece_grid[start_coord.y + 1][start_coord.x], dir_t::north))
		start_piece = start_piece | dir_t::south;

	piece_grid[start_coord.y][start_coord.x] = start_piece;

	dir_t provenance = {};
	coord_t current_coord = start_coord;
	std::size_t step_count = 0;

	do
	{
		piece_t const current_piece = piece_grid[current_coord.y][current_coord.x];
		if (has_dir(current_piece, dir_t::north) && provenance != dir_t::north)
		{
			--current_coord.y;
			provenance = dir_t::south;
		}
		else if (has_dir(current_piece, dir_t::west) && provenance != dir_t::west)
		{
			--current_coord.x;
			provenance = dir_t::east;
		}
		else if (has_dir(current_piece, dir_t::south) && provenance != dir_t::south)
		{
			++current_coord.y;
			provenance = dir_t::north;
		}
		else if (has_dir(current_piece, dir_t::east) && provenance != dir_t::east)
		{
			++current_coord.x;
			provenance = dir_t::west;
		}
		else
			throw std::invalid_argument(std::format("Invalid piece at ({}, {})", current_coord.x, current_coord.y));

		++step_count;
	} while (current_coord != start_coord);

	return std::to_string(step_count / 2);
}

static std::string day10_2(std::istream& i)
{
	std::size_t line_number = 1;

	coord_t start_coord = {};
	std::vector<std::vector<piece_t>> piece_grid;

	std::size_t line_size = 0;

	do
	{
		std::vector<piece_t> piece_line;
		do
		{
			int c = i.get();
			if (i.eof())
			{
				if (piece_line.size() != line_size)
					throw std::invalid_argument(std::format("Invalid input on line {}", line_number));

				break;
			}

			if (i.bad() || c == -1)
				throw std::invalid_argument(std::format("Invalid input on line {}", line_number));

			if (line_size != 0 && piece_line.size() == line_size)
				throw std::invalid_argument(std::format("Invalid input on line {}", line_number));


			piece_t const p = to_piece(static_cast<char>(c));
			if (p == piece_t::start)
				start_coord = { piece_line.size(), line_number - 1 };

			piece_line.push_back(p);
		} while (i && i.peek() != '\n');

		if (line_size == 0)
			line_size = piece_line.size();

		piece_grid.push_back(std::move(piece_line));
		piece_line.clear();

		i.ignore(1);
		++line_number;
	} while (i);

	piece_t start_piece = piece_t::ground;
	if (start_coord.x != 0 && has_dir(piece_grid[start_coord.y][start_coord.x - 1], dir_t::east))
		start_piece = start_piece | dir_t::west;

	if (start_coord.x != line_size - 1 && has_dir(piece_grid[start_coord.y][start_coord.x + 1], dir_t::west))
		start_piece = start_piece | dir_t::east;

	if (start_coord.y != 0 && has_dir(piece_grid[start_coord.y - 1][start_coord.x], dir_t::south))
		start_piece = start_piece | dir_t::north;

	if (start_coord.y != piece_grid.size() - 1 && has_dir(piece_grid[start_coord.y + 1][start_coord.x], dir_t::north))
		start_piece = start_piece | dir_t::south;

	piece_grid[start_coord.y][start_coord.x] = start_piece;

	std::vector<std::vector<status_t>> status_grid(piece_grid.size());
	for (std::vector<status_t>& status_line : status_grid)
		status_line.resize(line_size);
		
	dir_t provenance;
	coord_t current_coord = start_coord;
	dir_t inner_dir = {};

	{
		piece_t const starting_piece = piece_grid[start_coord.y][start_coord.x];
		switch (starting_piece)
		{
		case piece_t::bend_nw:
			provenance = dir_t::west;
			if (start_coord.y == piece_grid.size() - 1 || (start_coord.y == piece_grid.size() - 2 && piece_grid[start_coord.y + 1][start_coord.x] == piece_t::ground)
				|| start_coord.x == line_size - 1 || (start_coord.x == line_size - 2 && piece_grid[start_coord.y][start_coord.x + 1] == piece_t::ground))
			{
				inner_dir = dir_t::north;
			}
			break;
		case piece_t::bend_sw:
			provenance = dir_t::south;
			if (start_coord.y == 0 || (start_coord.y == 1 && piece_grid[start_coord.y - 1][start_coord.x] == piece_t::ground)
				|| start_coord.x == line_size - 1 || (start_coord.x == line_size - 2 && piece_grid[start_coord.y][start_coord.x + 1] == piece_t::ground))
			{
				inner_dir = dir_t::west;
			}
			break;
		case piece_t::bend_se:
			provenance = dir_t::east;
			if (start_coord.y == 0 || (start_coord.y == 1 && piece_grid[start_coord.y - 1][start_coord.x] == piece_t::ground)
				|| start_coord.x == 0 || (start_coord.x == 1 && piece_grid[start_coord.y][start_coord.x - 1] == piece_t::ground))
			{
				inner_dir = dir_t::south;
			}
			break;
		case piece_t::bend_ne:
			provenance = dir_t::east;
			if (start_coord.y == piece_grid.size() - 1 || (start_coord.y == piece_grid.size() - 2 && piece_grid[start_coord.y + 1][start_coord.x] == piece_t::ground)
				|| start_coord.x == 0 || (start_coord.x == 1 && piece_grid[start_coord.y][start_coord.x - 1] == piece_t::ground))
			{
				inner_dir = dir_t::south;
			}
			break;
		default: throw std::invalid_argument("Invalid starting piece");
		}
	}

	// Traverse the loop until we find an external edge
	if (inner_dir == dir_t{})
	{
		do
		{
			piece_t const current_piece = piece_grid[current_coord.y][current_coord.x];

			if (current_coord.x == 0 || (current_coord.x == 1 && piece_grid[current_coord.y][current_coord.x - 1] == piece_t::ground))
			{
				if (current_piece == piece_t::bend_ne)
				{
					start_coord = current_coord;
					inner_dir = dir_t::north;
					break;
				}
				else if (current_piece == piece_t::bend_se)
				{
					start_coord = current_coord;
					inner_dir = dir_t::south;
					break;
				}
			}
			else if (current_coord.x == line_size - 1 || (current_coord.x == line_size - 2 && piece_grid[current_coord.y][current_coord.x + 1] == piece_t::ground))
			{
				if (current_piece == piece_t::bend_nw)
				{
					start_coord = current_coord;
					inner_dir = dir_t::north;
					break;
				}
				else if (current_piece == piece_t::bend_sw)
				{
					start_coord = current_coord;
					inner_dir = dir_t::south;
					break;
				}
			}
			else if (current_coord.y == 0 && (current_coord.y == 1 && piece_grid[current_coord.y - 1][current_coord.x] == piece_t::ground))
			{
				if (current_piece == piece_t::bend_sw)
				{
					start_coord = current_coord;
					inner_dir = dir_t::west;
					break;
				}
				else if (current_piece == piece_t::bend_se)
				{
					start_coord = current_coord;
					inner_dir = dir_t::east;
					break;
				}
			}
			else if (current_coord.y == piece_grid.size() - 1 || (current_coord.y == piece_grid.size() - 2 && piece_grid[current_coord.y + 1][current_coord.x] == piece_t::ground))
			{
				if (current_piece == piece_t::bend_nw)
				{
					start_coord = current_coord;
					inner_dir = dir_t::west;
					break;
				}
				else if (current_piece == piece_t::bend_ne)
				{
					start_coord = current_coord;
					inner_dir = dir_t::east;
					break;
				}
			}

			if (has_dir(current_piece, dir_t::north) && provenance != dir_t::north)
			{
				--current_coord.y;
				provenance = dir_t::south;
			}
			else if (has_dir(current_piece, dir_t::west) && provenance != dir_t::west)
			{
				--current_coord.x;
				provenance = dir_t::east;
			}
			else if (has_dir(current_piece, dir_t::south) && provenance != dir_t::south)
			{
				++current_coord.y;
				provenance = dir_t::north;
			}
			else if (has_dir(current_piece, dir_t::east) && provenance != dir_t::east)
			{
				++current_coord.x;
				provenance = dir_t::west;
			}
			else
				throw std::invalid_argument(std::format("Invalid piece at ({}, {})", current_coord.x, current_coord.y));

		} while (current_coord != start_coord);
	}

	if (inner_dir == dir_t{})
		throw std::invalid_argument("Could not find loop inner");
	
	do
	{
		status_grid[current_coord.y][current_coord.x] = status_t::loop;

		piece_t const current_piece = piece_grid[current_coord.y][current_coord.x];
		rot_dir_t const rot_dir = get_rotation(current_piece, provenance);
		inner_dir = apply_rot(inner_dir, rot_dir);

		if (has_dir(current_piece, dir_t::north) && provenance != dir_t::north)
		{
			--current_coord.y;			
			provenance = dir_t::south;
		}
		else if (has_dir(current_piece, dir_t::west) && provenance != dir_t::west)
		{
			--current_coord.x;
			provenance = dir_t::east;
		}
		else if (has_dir(current_piece, dir_t::south) && provenance != dir_t::south)
		{
			++current_coord.y;
			provenance = dir_t::north;
		}
		else if (has_dir(current_piece, dir_t::east) && provenance != dir_t::east)
		{
			++current_coord.x;
			provenance = dir_t::west;
		}
		else
			throw std::invalid_argument(std::format("Invalid piece at ({}, {})", current_coord.x, current_coord.y));

		piece_t const next_piece = piece_grid[current_coord.y][current_coord.x];

		auto write_dir = [&](dir_t dir)
		{
			switch (dir)
			{
			case dir_t::north:
				if (current_coord.y != 0 && status_grid[current_coord.y - 1][current_coord.x] != status_t::loop)
					status_grid[current_coord.y - 1][current_coord.x] = status_t::inside;
				if (current_coord.y != piece_grid.size() - 1 && status_grid[current_coord.y + 1][current_coord.x] != status_t::loop)
					status_grid[current_coord.y + 1][current_coord.x] = status_t::outside;

				break;
			case dir_t::west:
				if (current_coord.x != 0 && status_grid[current_coord.y][current_coord.x - 1] != status_t::loop)
					status_grid[current_coord.y][current_coord.x - 1] = status_t::inside;
				if (current_coord.x != line_size - 1 && status_grid[current_coord.y][current_coord.x + 1] != status_t::loop)
					status_grid[current_coord.y][current_coord.x + 1] = status_t::outside;
				break;
			case dir_t::south:
				if (current_coord.y != piece_grid.size() - 1 && status_grid[current_coord.y + 1][current_coord.x] != status_t::loop)
					status_grid[current_coord.y + 1][current_coord.x] = status_t::inside;
				if (current_coord.y != 0 && status_grid[current_coord.y - 1][current_coord.x] != status_t::loop)
					status_grid[current_coord.y - 1][current_coord.x] = status_t::outside;
				break;
			case dir_t::east:
				if (current_coord.x != line_size - 1 && status_grid[current_coord.y][current_coord.x + 1] != status_t::loop)
					status_grid[current_coord.y][current_coord.x + 1] = status_t::inside;
				if (current_coord.x != 0 && status_grid[current_coord.y][current_coord.x - 1] != status_t::loop)
					status_grid[current_coord.y][current_coord.x - 1] = status_t::outside;
				break;
			}
		};

		write_dir(inner_dir);

		dir_t const other_inner = get_other_dir(next_piece, inner_dir);
		if (other_inner != dir_t{})
			write_dir(other_inner);
	} while (current_coord != start_coord);

	// Fill inner pools
	for (std::size_t j = 1; j < status_grid.size() - 1; ++j)
	{
		for (std::size_t i = 1; i < line_size - 1; ++i)
		{
			if (status_grid[j][i] != status_t::unset)
				continue;

			if (status_grid[j][i - 1] == status_t::inside
				|| status_grid[j][i + 1] == status_t::inside
				|| status_grid[j - 1][i] == status_t::inside
				|| status_grid[j + 1][i] == status_t::inside)
				status_grid[j][i] = status_t::inside;
		}
	}

	std::size_t const count = std::accumulate(status_grid.begin(), status_grid.end(), 0ull, [](std::size_t acc, std::vector<status_t> const& status_line)
	{
		return acc + std::accumulate(status_line.begin(), status_line.end(), 0ull, [](std::size_t acc, status_t s) { return s == status_t::inside ? acc + 1 : acc; });
	});

	std::string output;
	for (std::size_t j = 0; j < piece_grid.size(); ++j)
	{
		for (std::size_t i = 0; i < line_size; ++i)
		{
			if (status_grid[j][i] == status_t::inside)
				output += 'I';
			else if (status_grid[j][i] == status_t::outside)
				output += 'O';
			else
				output += to_char(piece_grid[j][i]);
		}
		output += '\n';
	}

	return output + "Answer: " + std::to_string(count);
}

solver_registery day10(10, &day10_1, &day10_2);