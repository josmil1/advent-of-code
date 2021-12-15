#include "Puzzles.hpp"

namespace
{
uint64_t puzzle_01_1(std::ifstream &in_file)
{
	// Parse input
	std::string line;
	uint32_t    increase_count = 0;
	int32_t     prev_number    = -1;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			int32_t input_number = std::stoi(line);
			int32_t difference   = input_number - prev_number;
			if (prev_number > 0 && difference > 0)
			{
				// std::cout << input_number << " - " << prev_number << " = " <<
				// difference << std::endl;
				increase_count++;
			}
			prev_number = input_number;
		}
	}

	return increase_count;
}

uint64_t puzzle_01_2(std::ifstream &in_file)
{
	// Parse input
	std::string           line;
	uint32_t              increase_count = 0;
	constexpr uint32_t    window_size    = 3;
	uint32_t              count = 0, sum_index = 0;
	std::vector<uint32_t> sums;
	sums.resize(window_size);
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			uint32_t input_number = std::stoul(line);

			if (count < window_size)
			{
				// Initialise sums
				for (uint32_t i = 0; i <= count; i++)
				{
					sums[i] += input_number;
					// std::cout << "sum " << i << " + " << input_number << " = " <<
					// sums[i] << std::endl;
				}
			}
			else
			{
				// Calculate differences and update sums
				sum_index = (sum_index > window_size - 2) ? 0 : sum_index + 1;
				const uint32_t prev_sum_index =
				    (sum_index == 0) ? window_size - 1 : sum_index - 1;

				for (uint32_t i = 0; i < window_size; i++)
				{
					// Add to all sums except the previous one, which will be
					// set to the new number at the end
					if (i != prev_sum_index)
					{
						sums[i] += input_number;
					}
					// std::cout << "sums " << i << " ( " << sums[i] << ") + " <<
					// input_number << " = " << sums[i] + input_number << std::endl;
				}

				const int32_t difference =
				    static_cast<int32_t>(sums[sum_index] - sums[prev_sum_index]);
				std::cout << "sums[" << sum_index << "] (" << sums[sum_index]
				          << ") - sums[" << prev_sum_index << "] ("
				          << sums[prev_sum_index] << ") = " << difference << std::endl;
				if (difference > 0)
				{
					increase_count++;
				}

				sums[prev_sum_index] = input_number;
			}

			count++;
		}
	}

	return increase_count;
}
}        // namespace

uint64_t puzzle_01(std::ifstream &in_file)
{
	return puzzle_01_1(in_file);
}
