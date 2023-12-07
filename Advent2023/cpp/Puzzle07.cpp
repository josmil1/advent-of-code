#include "Puzzles.hpp"

namespace
{
enum HandType : uint16_t
{
	Unknown   = 7,
	FiveKind  = 6,        // All 5 cards have the same label: AAAAA
	FourKind  = 5,        // 4 cards have the same label, and 1 card has a different label: AA8AA
	FullHouse = 4,        // 3 cards have the same label, and the remaining 2 cards share a different label: 23332
	ThreeKind = 3,        // 3 cards have the same label, and the remaining 2 cards are each different from any other card in the hand: TTT98
	TwoPair   = 2,        // 2 cards share one label, 2 other cards share a second label, and the remaining 1 card has a third label: 23432
	OnePair   = 1,        // 2 cards share one label, and the other 3 cards have a different label from the pair and each other: A23A4
	HighCard  = 0         // All 5 cards' labels are distinct
};

const std::unordered_map<char, uint16_t> card_values{
    {'A', 13},
    {'K', 12},
    {'Q', 11},
    {'J', 10},
    {'T', 9},
    {'9', 8},
    {'8', 7},
    {'7', 6},
    {'6', 5},
    {'5', 4},
    {'4', 3},
    {'3', 2},
    {'2', 1}};

struct Hand
{
	std::string cards;
	uint64_t    bid;
	HandType    type{Unknown};

	Hand(const std::string &c, uint64_t b) :
	    cards(c), bid(b)
	{
		std::unordered_map<char, uint16_t> counts;

		for (auto &c : cards)
		{
			counts[c]++;
		}

		if (1 == counts.size())
		{
			type = FiveKind;
		}
		else if (2 == counts.size())
		{
			auto it         = counts.begin();
			auto first_size = it->second;
			std::advance(it, 1);
			auto second_size = it->second;

			auto count_diff = std::abs(static_cast<int16_t>(first_size) - second_size);

			if (3 == count_diff)
			{
				type = FourKind;
			}
			else if (1 == count_diff)
			{
				type = FullHouse;
			}
			else
			{
				type = Unknown;
			}
		}
		else if (3 == counts.size())
		{
			for (auto &it : counts)
			{
				if (it.second == 3)
				{
					type = ThreeKind;
					break;
				}
				else if (it.second == 2)
				{
					type = TwoPair;
					break;
				}
			}
		}
		else if (4 == counts.size())
		{
			type = OnePair;
		}
		else
		{
			type = HighCard;
		}
	}

	bool operator<(const Hand &other) const
	{
		if (type == other.type)
		{
			for (int i = 0; i < cards.size(); i++)
			{
				char this_card  = cards[i];
				char other_card = other.cards[i];

				if (this_card == other_card)
				{
					continue;
				}
				else
				{
					return card_values.at(this_card) < card_values.at(other_card);
				}
			}
		}
		else
		{
			return (type < other.type);
		}

		return true;
	}
};

std::vector<Hand> parse_hands(std::ifstream &in_file)
{
	std::string line;
	std::string hand;
	uint64_t    bid;

	std::vector<Hand> hands;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);

			ss >> hand >> bid;
			hands.push_back({hand, bid});
		}
	}

	return hands;
}

uint64_t puzzle_07_1(std::ifstream &in_file)
{
	uint64_t sum{0};

	auto hands = parse_hands(in_file);

	for (auto &h : hands)
	{
		if (h.type < 0 || h.type > 7)
		{
			std::cout << h.cards << " " << h.type << std::endl;
		}
	}

	std::stable_sort(hands.begin(), hands.end());

	for (int i = 0; i < hands.size(); i++)
	{
		sum += (i + 1) * hands[i].bid;
	}

	return sum;
}

uint64_t puzzle_07_2(std::ifstream &in_file)
{
	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::cout << line << std::endl;
		}
	}

	return 0;
}
}        // namespace

uint64_t puzzle_07(std::ifstream &in_file)
{
	return puzzle_07_1(in_file);
}
