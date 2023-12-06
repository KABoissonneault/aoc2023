#define _CRT_SECURE_NO_WARNINGS

#include "maths.h"
#include "registry.h"

#include <iostream>
#include <vector>
#include <format>
#include <algorithm>
#include <string>

static std::string day5_1(std::istream& i)
{
	int line_number = 1;
		
	auto skip_line = [&i, &line_number]
	{		
		i.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		++line_number;
	};

	// Parse initial seeds
	i.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	std::vector<std::uint64_t> ids;
	std::uint64_t id;
	while (i.peek() != '\n' && (i >> id))
		ids.push_back(id);

	if (i.fail())
		throw std::invalid_argument(std::format("Invalid seeds on line {}", line_number));
	
	skip_line(); // End line

	skip_line(); // Skip empty line

	// Parse other maps
	
	struct mapping
	{
		std::uint64_t destination_start;
		std::uint64_t source_start;
		std::uint64_t range_length;
	};

	do
	{
		skip_line(); // Skip header

		std::vector<mapping> mappings;

		std::string line;
		while (std::getline(i, line) && !line.empty())
		{
			std::uint64_t destination_start;
			std::uint64_t source_start;
			std::uint64_t range_length;
			int const parsed = std::sscanf(line.c_str(), "%llu %llu %llu", &destination_start, &source_start, &range_length);
			if(parsed != 3)
				throw std::invalid_argument(std::format("Invalid mapping on line {}", line_number));

			mappings.emplace_back(destination_start, source_start, range_length);
			++line_number;
		}

		std::transform(ids.begin(), ids.end(), ids.begin(), [&mappings](std::uint64_t id)
		{
			auto const it_found = std::ranges::find_if(mappings, [id](const mapping& m)
			{
				return id >= m.source_start && id <= m.source_start + m.range_length;
			});
			
			if (it_found != mappings.end())
			{
				mapping const& m = *it_found;
				std::uint64_t const offset = id - m.source_start;
				return m.destination_start + offset;
			}

			return id;
		});	
	} while (!i.eof());

	std::uint64_t const min_id = *std::ranges::min_element(ids);

	return std::to_string(min_id);
}

static std::string day5_2(std::istream& i)
{
	int line_number = 1;

	auto skip_line = [&i, &line_number]
	{
		i.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		++line_number;
	};

	// Parse initial seeds
	i.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	struct id_range
	{
		std::uint64_t start;
		std::uint64_t length;

		[[nodiscard]] constexpr std::uint64_t end() const noexcept { return start + length; }
	};

	std::vector<id_range> ids;
	std::uint64_t id_start;
	std::uint64_t id_length;
	while (i.peek() != '\n' && (i >> id_start) && (i >> id_length))
		ids.emplace_back(id_start, id_length);

	if (i.fail())
		throw std::invalid_argument(std::format("Invalid seeds on line {}", line_number));

	skip_line(); // End line

	skip_line(); // Skip empty line

	// Parse other maps

	struct mapping
	{
		std::uint64_t destination_start;
		std::uint64_t source_start;
		std::uint64_t range_length;

		constexpr id_range source_intersection(id_range const& r) const
		{
			std::uint64_t const intersection_start = maths::max_value(source_start, r.start);
			std::uint64_t const intersection_end = maths::min_value(source_start + range_length, r.start + r.length);
			if (intersection_start >= intersection_end)
				return {};

			return { intersection_start, intersection_end - intersection_start };
		}
	};

	do
	{
		skip_line(); // Skip header

		std::vector<mapping> mappings;
		std::vector<id_range> new_ranges;

		std::string line;
		while (std::getline(i, line) && !line.empty())
		{
			std::uint64_t destination_start;
			std::uint64_t source_start;
			std::uint64_t range_length;
			int const parsed = std::sscanf(line.c_str(), "%llu %llu %llu", &destination_start, &source_start, &range_length);
			if (parsed != 3)
				throw std::invalid_argument(std::format("Invalid mapping on line {}", line_number));

			mapping const m{ destination_start, source_start, range_length };

			std::size_t const current_size = ids.size();
			for (std::size_t i = 0; i < current_size; ++i)
			{
				id_range& source_range = ids[i];

				id_range const intersection = m.source_intersection(source_range);
				if (intersection.length == 0)
					continue;

				if (intersection.start != source_range.start)
					ids.emplace_back(source_range.start, intersection.start - source_range.start);

				if (intersection.end() != source_range.end())
					ids.emplace_back(intersection.end(), source_range.end() - intersection.end());

				source_range.length = 0; // Remove this part, already mapped

				std::uint64_t offset = intersection.start - m.source_start;
				new_ranges.emplace_back(m.destination_start + offset, intersection.length);
			}

			// Remove mapped sections
			std::erase_if(ids, [](id_range const& r) { return r.length == 0; });

			++line_number;
		}

		new_ranges.insert(new_ranges.end(), ids.begin(), ids.end());

		ids = std::move(new_ranges);
	} while (!i.eof());

	std::uint64_t const min_id = std::ranges::min_element(ids, std::less<std::uint64_t>(), &id_range::start)->start;

	return std::to_string(min_id);
}

solver_registery day5(5, &day5_1, &day5_2);
