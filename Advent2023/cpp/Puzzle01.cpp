#include "Puzzles.hpp"

#include <set>

namespace
{
uint64_t puzzle_01_1(std::ifstream &in_file)
{
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
	std::string line;

	constexpr uint32_t top_elf_count = 3;

	std::vector<uint64_t> top_elf_sums;
	uint64_t              cur_sum{0};

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
			if (top_elf_sums.size() < top_elf_count)
			{
				top_elf_sums.push_back(cur_sum);
			}
			else
			{
				std::sort(top_elf_sums.begin(), top_elf_sums.end(), std::less<>());

				for (uint32_t i = 0; i < top_elf_count; i++)
				{
					if (cur_sum > top_elf_sums[i])
					{
						top_elf_sums[i] = cur_sum;
						break;
					}
				}
			}

			cur_sum = 0;
		}
	}

	return std::accumulate(top_elf_sums.begin(), top_elf_sums.end(), 0);
}
}        // namespace

uint64_t puzzle_01(std::ifstream &in_file)
{
	return puzzle_01_1(in_file);
}
