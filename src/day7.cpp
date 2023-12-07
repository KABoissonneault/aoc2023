#include "registry.h"

#include <iostream>
#include <format>
#include <vector>
#include <algorithm>
#include <functional>

namespace
{
	enum class card_kind
	{
		two = 2,
		three,
		four,
		five,
		six,
		seven,
		eight,
		nine,
		ten,
		jack,
		queen,
		king,
		ace,
	};

	constexpr std::size_t kind_count = static_cast<std::size_t>(card_kind::ace) - 1;

	std::istream& operator>>(std::istream& i, card_kind& k)
	{
		char c = i.get();
		if (!i)
			return i;

		if (c >= '2' && c <= '9')
			k = static_cast<card_kind>(c - '0');
		else if (c == 'T')
			k = card_kind::ten;
		else if (c == 'J')
			k = card_kind::jack;
		else if (c == 'Q')
			k = card_kind::queen;
		else if (c == 'K')
			k = card_kind::king;
		else if (c == 'A')
			k = card_kind::ace;
		else
			i.setstate(std::ios_base::failbit);

		return i;
	}

	std::strong_ordering operator<=>(card_kind lhs, card_kind rhs) noexcept
	{
		return static_cast<int>(lhs) <=> static_cast<int>(rhs);
	}

	enum class card_kind2
	{
		joker = 1,
		two,
		three,
		four,
		five,
		six,
		seven,
		eight,
		nine,
		ten,
		queen,
		king,
		ace,
	};

	constexpr std::size_t kind2_count = static_cast<std::size_t>(card_kind2::ace) - 1;

	std::istream& operator>>(std::istream& i, card_kind2& k)
	{
		char c = i.get();
		if (!i)
			return i;

		if (c >= '2' && c <= '9')
			k = static_cast<card_kind2>(c - '0');
		else if (c == 'T')
			k = card_kind2::ten;
		else if (c == 'J')
			k = card_kind2::joker;
		else if (c == 'Q')
			k = card_kind2::queen;
		else if (c == 'K')
			k = card_kind2::king;
		else if (c == 'A')
			k = card_kind2::ace;
		else
			i.setstate(std::ios_base::failbit);

		return i;
	}

	std::strong_ordering operator<=>(card_kind2 lhs, card_kind2 rhs) noexcept
	{
		return static_cast<int>(lhs) <=> static_cast<int>(rhs);
	}

	enum class hand_type
	{
		high_card,
		one_pair,
		two_pair,
		three_of_a_kind,
		full_house,
		four_of_a_kind,
		five_of_a_kind,
	};

	std::strong_ordering operator<=>(hand_type lhs, hand_type rhs) noexcept
	{
		return static_cast<int>(lhs) <=> static_cast<int>(rhs);
	}

	struct play_t
	{
		card_kind hand[5];
		int bid;
		hand_type type;
	};

	bool operator==(play_t const& lhs, play_t const& rhs)
	{
		if (lhs.type != rhs.type)
			return false;

		for (std::size_t i = 0; i < 5; ++i)
		{
			if (lhs.hand[i] != rhs.hand[i])
				return false;
		}

		return true;
	}

	std::weak_ordering operator<=>(play_t const& lhs, play_t const& rhs)
	{
		if (lhs.type != rhs.type)
			return lhs.type <=> rhs.type;

		for (std::size_t i = 0; i < 5; ++i)
		{
			if (lhs.hand[i] != rhs.hand[i])
				return lhs.hand[i] <=> rhs.hand[i];
		}

		return std::weak_ordering::equivalent;
	}

	hand_type get_hand_type(card_kind const(&hand)[5])
	{
		int counts[kind_count] = {};

		for (card_kind k : hand)
			++counts[static_cast<std::size_t>(k) - static_cast<std::size_t>(card_kind::two)];

		int highest_count = 0;
		int penultimate_count = 0;
		for (std::size_t i = 0; i < kind_count; ++i)
		{
			if (counts[i] >= highest_count)
			{
				penultimate_count = highest_count;
				highest_count = counts[i];
			}
			else if (counts[i] >= penultimate_count)
			{
				penultimate_count = counts[i];
			}
		}

		if (highest_count == 5)
			return hand_type::five_of_a_kind;
		else if (highest_count == 4)
			return hand_type::four_of_a_kind;
		else if (highest_count == 3)
		{
			if (penultimate_count == 2)
				return hand_type::full_house;
			else
				return hand_type::three_of_a_kind;
		}
		else if (highest_count == 2)
		{
			if (penultimate_count == 2)
				return hand_type::two_pair;
			else
				return hand_type::one_pair;
		}
		else
			return hand_type::high_card;
	}

	struct play2_t
	{
		card_kind2 hand[5];
		int bid;
		hand_type type;
	};

	bool operator==(play2_t const& lhs, play2_t const& rhs)
	{
		if (lhs.type != rhs.type)
			return false;

		for (std::size_t i = 0; i < 5; ++i)
		{
			if (lhs.hand[i] != rhs.hand[i])
				return false;
		}

		return true;
	}

	std::weak_ordering operator<=>(play2_t const& lhs, play2_t const& rhs)
	{
		if (lhs.type != rhs.type)
			return lhs.type <=> rhs.type;

		for (std::size_t i = 0; i < 5; ++i)
		{
			if (lhs.hand[i] != rhs.hand[i])
				return lhs.hand[i] <=> rhs.hand[i];
		}

		return std::weak_ordering::equivalent;
	}

	hand_type get_hand_type(card_kind2 const(&hand)[5])
	{
		int counts[kind_count] = {};

		for (card_kind2 k : hand)
			++counts[static_cast<std::size_t>(k) - static_cast<std::size_t>(card_kind2::joker)];

		int const joker_count = counts[0];

		int highest_count = 0;
		int penultimate_count = 0;
		for (std::size_t i = 1; i < kind_count; ++i)
		{
			if (counts[i] >= highest_count)
			{
				penultimate_count = highest_count;
				highest_count = counts[i];
			}
			else if (counts[i] >= penultimate_count)
			{
				penultimate_count = counts[i];
			}
		}

		if (highest_count + joker_count == 5)
			return hand_type::five_of_a_kind;
		else if (highest_count + joker_count == 4)
			return hand_type::four_of_a_kind;
		else if (highest_count + joker_count == 3)
		{
			if (penultimate_count == 2)
				return hand_type::full_house;
			else
				return hand_type::three_of_a_kind;
		}
		else if (highest_count + joker_count == 2)
		{
			if (penultimate_count == 2)
				return hand_type::two_pair;
			else
				return hand_type::one_pair;
		}
		else
			return hand_type::high_card;
	}
}

static std::string day7_1(std::istream& i)
{
	int line_number = 1;

	std::vector<play_t> plays;

	while (i)
	{
		play_t play;
		for (int k = 0; k < 5; ++k)
		{
			if (!(i >> play.hand[k]))
				throw std::invalid_argument(std::format("Invalid card on line {}", line_number));			
		}

		if(!(i >> play.bid))
			throw std::invalid_argument(std::format("Invalid card on line {}", line_number));

		i.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		play.type = get_hand_type(play.hand);

		plays.push_back(play);

		++line_number;
	}

	std::ranges::sort(plays);

	std::size_t result = 0;

	for (std::size_t rank = 0; rank < plays.size(); ++rank)
	{
		result += (rank + 1) * plays[rank].bid;
	}

	return std::to_string(result);
}

static std::string day7_2(std::istream& i)
{
	int line_number = 1;

	std::vector<play2_t> plays;

	while (i)
	{
		play2_t play;
		for (int k = 0; k < 5; ++k)
		{
			if (!(i >> play.hand[k]))
				throw std::invalid_argument(std::format("Invalid card on line {}", line_number));
		}

		if (!(i >> play.bid))
			throw std::invalid_argument(std::format("Invalid card on line {}", line_number));

		i.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		play.type = get_hand_type(play.hand);

		plays.push_back(play);

		++line_number;
	}

	std::ranges::sort(plays);

	std::size_t result = 0;

	for (std::size_t rank = 0; rank < plays.size(); ++rank)
	{
		result += (rank + 1) * plays[rank].bid;
	}

	return std::to_string(result);
}

solver_registery day7(7, &day7_1, &day7_2);
