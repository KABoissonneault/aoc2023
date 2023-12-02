#include "parse.h"

#include <charconv>
#include <format>

int to_int(std::string_view s)
{
	int value;

	if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc())
		return value;

	throw std::invalid_argument(std::format("String '{}' was not a number", s));
}
