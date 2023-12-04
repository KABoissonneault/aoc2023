#pragma once

#include <type_traits>

namespace maths
{
	template<typename T, typename U>
	inline constexpr auto min_value(T lhs, U rhs) noexcept -> std::common_type_t<T, U>
	{
		return lhs < rhs ? lhs : rhs;
	}

	template<typename T, typename U>
	inline constexpr auto max_value(T lhs, U rhs) noexcept -> std::common_type_t<T, U>
	{
		return lhs < rhs ? rhs : lhs;
	}
}
