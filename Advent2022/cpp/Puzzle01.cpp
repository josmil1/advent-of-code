#include "Puzzles.hpp"

#include <set>

namespace
{
uint64_t puzzle_01_1(std::ifstream &in_file)
{
	// Parse input
	std::string line;

	uint64_t max     = 0;
	uint64_t cur_sum = 0;

	while (!in_file.eof())
	{
		std::getline(in_file, line);

		if (line.size() > 0)
		{
			int64_t input_number = std::stol(line);
			cur_sum += input_number;
		}

		if (line.size() == 0 || in_file.eof())
		{
			max     = std::max(max, cur_sum);
			cur_sum = 0;
		}
	}

	return max;
}

uint64_t puzzle_01_2(std::ifstream &in_file)
{
	// Parse input
	std::string line;

	std::set<uint64_t> maxes;
	uint64_t           cur_sum   = 0;
	constexpr uint32_t max_count = 3;

	while (!in_file.eof())
	{
		std::getline(in_file, line);

		if (line.size() > 0)
		{
			int64_t input_number = std::stol(line);
			cur_sum += input_number;
		}

		if (line.size() == 0 || in_file.eof())
		{
			if (maxes.size() < max_count || cur_sum > *maxes.begin())
			{
				if (maxes.size() == max_count)
				{
					maxes.erase(maxes.begin());
				}
				maxes.insert(cur_sum);
			}
			cur_sum = 0;
		}
	}

	return accumulate(maxes.begin(), maxes.end(), 0);
}
}        // namespace

uint64_t puzzle_01(std::ifstream &in_file)
{
	return puzzle_01_2(in_file);
}
