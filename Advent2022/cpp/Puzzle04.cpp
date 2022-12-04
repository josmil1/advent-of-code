#include "Puzzles.hpp"

namespace
{
uint64_t puzzle_04_1(std::ifstream &in_file)
{
	std::string line;

	uint64_t overlap_count = 0;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Parse input
			std::stringstream ss(line);
			uint64_t          first_min, first_max;
			uint64_t          second_min, second_max;
			char              bin;        // Discard separators
			ss >> first_min >> bin >> first_max >> bin >> second_min >> bin >> second_max;

			// Ensure first range is larger
			if (first_max - first_min < second_max - second_min)
			{
				std::swap(first_min, second_min);
				std::swap(first_max, second_max);
			}

			// Find full overlap
			if (second_min >= first_min && second_max <= first_max)
			{
				overlap_count++;
			}
		}
	}

	return overlap_count;
}

uint64_t puzzle_04_2(std::ifstream &in_file)
{
	std::string line;

	uint64_t overlap_count = 0;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Parse input
			std::stringstream ss(line);
			uint64_t          first_min, first_max;
			uint64_t          second_min, second_max;
			char              bin;        // Discard separators
			ss >> first_min >> bin >> first_max >> bin >> second_min >> bin >> second_max;

			// Ensure first range starts earlier
			if (first_min > second_min)
			{
				std::swap(first_min, second_min);
				std::swap(first_max, second_max);
			}

			// Find partial overlap
			if (second_min <= first_max)
			{
				overlap_count++;
			}
		}
	}

	return overlap_count;
}
}        // namespace

uint64_t puzzle_04(std::ifstream &in_file)
{
	return puzzle_04_2(in_file);
}
