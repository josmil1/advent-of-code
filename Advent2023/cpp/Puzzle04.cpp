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

uint64_t puzzle_04(std::ifstream &in_file)
{
	return puzzle_04_1(in_file);
}
