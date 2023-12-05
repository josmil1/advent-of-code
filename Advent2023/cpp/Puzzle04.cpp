#include "Puzzles.hpp"

namespace
{
uint64_t puzzle_04_1(std::ifstream &in_file)
{
	uint64_t sum{0};

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Parse card and check winning (repeat) numbers
			std::stringstream ss(line);

			std::unordered_map<uint32_t, bool> numbers;
			int                                card_id;
			std::string                        str;
			uint32_t                           num;
			uint64_t                           count{0};

			// Card <id>:
			ss >> str >> card_id >> str;

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
						numbers[num] = true;
					}
				}
			}

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

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Parse card and check winning (repeat) numbers
			std::stringstream ss(line);

			int         card_id;
			std::string str;
			uint32_t    num;

			// Card <id>:
			ss >> str >> card_id >> str;
			card_count[card_id]++;

			std::vector<uint32_t> number_list;
			while (ss >> str)
			{
				if (str != "|")
				{
					num = static_cast<uint32_t>(std::stol(str));
					number_list.push_back(num);
				}
			}

			for (int i = 0; i < card_count[card_id]; i++)
			{
				std::unordered_map<uint32_t, bool> numbers;
				uint64_t                           count{0};

				for (auto &num : number_list)
				{
					if (numbers.count(num))
					{
						// Number appeared in winning list
						count++;
						card_count[card_id + count]++;
					}
					else
					{
						// New number
						numbers[num] = true;
					}
				}
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
