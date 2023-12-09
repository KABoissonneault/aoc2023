#include "registry.h"

#include <iostream>
#include <vector>
#include <algorithm>

using i64 = long long;

static i64 find_projected_next_number(std::vector<i64> const& numbers)
{
	bool any_non_zero = false;

	std::vector<i64> derivatives;
	for (std::size_t i = 0; i < numbers.size() - 1; ++i)
	{
		i64 const d = numbers[i + 1] - numbers[i];
		if (d != 0)
			any_non_zero = true;
		derivatives.push_back(d);
	}

	if (!any_non_zero)
		return numbers.back();

	i64 const next_projected_derivative = find_projected_next_number(derivatives);
	return numbers.back() + next_projected_derivative;
}

static i64 find_projected_prev_number(std::vector<i64> const& numbers)
{
	bool any_non_zero = false;

	std::vector<i64> derivatives;
	for (std::size_t i = 0; i < numbers.size() - 1; ++i)
	{
		i64 const d = numbers[i + 1] - numbers[i];
		if (d != 0)
			any_non_zero = true;
		derivatives.push_back(d);
	}

	if (!any_non_zero)
		return numbers.front();

	i64 const next_projected_derivative = find_projected_prev_number(derivatives);
	return numbers.front() - next_projected_derivative;
}

static std::string day9_1(std::istream& i)
{
	i64 result = 0;

	do
	{
		std::vector<i64> numbers;
		do
		{
			i64 number;
			if(i >> number)
				numbers.push_back(number);
		} while (i && i.peek() != '\n');

		if (!i.bad() && !numbers.empty())
		{
			result += find_projected_next_number(numbers);

			i.ignore(1);
		}
	} while (!i.eof());

	return std::to_string(result);
}

static std::string day9_2(std::istream& i)
{
	i64 result = 0;

	do
	{
		std::vector<i64> numbers;
		do
		{
			i64 number;
			if (i >> number)
				numbers.push_back(number);
		} while (i && i.peek() != '\n');

		if (!i.bad() && !numbers.empty())
		{
			result += find_projected_prev_number(numbers);

			i.ignore(1);
		}
	} while (!i.eof());

	return std::to_string(result);
}

solver_registery day9(9, &day9_1, &day9_2);
