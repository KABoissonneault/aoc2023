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

std::size_t to_size_t(std::string_view s)
{
	std::size_t value;

	if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc())
		return value;

	throw std::invalid_argument(std::format("String '{}' was not a number", s));
}
