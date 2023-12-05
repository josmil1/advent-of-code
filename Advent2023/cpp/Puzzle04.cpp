#include "Puzzles.hpp"

#include <set>

namespace
{
uint32_t count_winning_numbers(std::stringstream &ss)
{
	uint32_t    num;
	std::string str;
	uint32_t    count{0};

	std::set<uint32_t> numbers;

	while (ss >> str)
	{
		if (str != "|")
		{
			num = static_cast<uint32_t>(std::stol(str));
			if (numbers.count(num))
			{
				// Number appeared in winning list
				count++;
			}
			else
			{
				// New number
				numbers.insert(num);
			}
		}
	}

	return count;
}

uint64_t puzzle_04_1(std::ifstream &in_file)
{
	uint64_t sum{0};

	std::string line;
	std::string str;
	uint32_t    card_id;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Parse card and check winning (repeat) numbers
			std::stringstream ss(line);

			// Card <id>:
			ss >> str >> card_id >> str;

			auto count = count_winning_numbers(ss);

			// The first match makes the card worth one point,
			// each match after the first doubles the point value
			if (count > 0)
			{
				sum += std::pow(2, count - 1);
			}
		}
	}

	return sum;
}

uint64_t puzzle_04_2(std::ifstream &in_file)
{
	std::unordered_map<uint32_t, uint32_t> card_count;

	std::string line;
	std::string str;
	uint32_t    card_id;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Parse card and check winning (repeat) numbers
			std::stringstream ss(line);

			// Card <id>:
			ss >> str >> card_id >> str;

			auto count = count_winning_numbers(ss);

			// Track copies of each card
			card_count[card_id]++;
			for (int i = 0; i < count; i++)
			{
				card_count[card_id + i + 1] += card_count[card_id];
			}
		}
	}

	return std::accumulate(card_count.begin(), card_count.end(), 0,
	                       [](uint32_t value, const std::unordered_map<uint32_t, uint32_t>::value_type &p) { return value + p.second; });
}
}        // namespace

uint64_t puzzle_04(std::ifstream &in_file)
{
	return puzzle_04_2(in_file);
}
