#include "registry.h"
#include "parse.h"

#include <iostream>
#include <vector>
#include <string>

static std::string day6_1(std::istream& i)
{
	i.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	std::vector<std::size_t> times;
	std::size_t number;
	while (i.peek() != '\n' && (i >> number))
		times.push_back(number);

	i.ignore(1); // End line

	i.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	std::vector<std::size_t> distances;
	while (i.peek() != '\n' && (i >> number))
		distances.push_back(number);

	if (times.size() != distances.size())
		throw std::invalid_argument("Input time and distance mismatch");

	int product = 1;

	std::size_t const val_count = times.size();
	for (std::size_t i = 0; i < val_count; ++i)
	{
		std::size_t const time = times[i];
		std::size_t const distance = distances[i];

		int win_count = 0;

		for (std::size_t t = 0; t < time; ++t)
		{
			std::size_t const d = t * (time - t);
			if (d > distance)
				++win_count;
		}

		product *= win_count;
	}

	return std::to_string(product);
}

static std::string day6_2(std::istream& i)
{
	i.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	std::string number_str;
	while (i && i.peek() != '\n')
	{
		char c = i.get();
		if(std::isdigit(c))
			number_str += c;
	}
	
	i.ignore(1);
	
	std::size_t const time = to_size_t(number_str);

	i.ignore(std::numeric_limits<std::streamsize>::max(), ':');

	number_str.clear();
	while (i && i.peek() != '\n')
	{
		char c = i.get();
		if (std::isdigit(c))
			number_str += c;
	}

	std::size_t const distance = to_size_t(number_str);

	for (std::size_t t = 0; t < time; ++t)
	{
		std::size_t const d = t * (time - t);
		if (d > distance)
			return std::to_string((time + 1) - 2 * t);
	}

	throw std::invalid_argument("No solution");
}

solver_registery day6(6, &day6_1, &day6_2);