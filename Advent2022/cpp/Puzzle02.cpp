#include "Puzzles.hpp"

#include <set>

namespace
{
constexpr uint32_t in_offset  = 'A';
constexpr uint32_t out_offset = 'X';

bool beats(uint32_t in, uint32_t out)
{
	return out == (in + 1) % 3;
}

uint64_t puzzle_02_1(std::ifstream &in_file)
{
	// Parse input
	std::string line;

	uint64_t score = 0;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			char              in_c, out_c;
			ss >> in_c >> out_c;

			uint32_t in  = in_c - in_offset;
			uint32_t out = out_c - out_offset;

			score += out + 1;
			if (in == out)
			{
				// Draw
				score += 3;
			}
			else if (beats(in, out))
			{
				// Win
				score += 6;
			}
		}
	}

	return score;
}

uint64_t puzzle_02_2(std::ifstream &in_file)
{
	// Parse input
	std::string line;

	uint64_t score = 0;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			char              in_c, result_c;
			ss >> in_c >> result_c;

			uint32_t in     = in_c - in_offset;
			uint32_t result = result_c - out_offset;
			uint32_t out    = 0;

			switch (result)
			{
				case 0:
					// Lose
					out = (in + 2) % 3;
					break;
				case 1:
					// Draw
					out = in;
					score += 3;
					break;
				case 2:
					// Win
					out = (in + 1) % 3;
					score += 6;
					break;
				default:
					std::cout << "Error: unknown result " << result_c << std::endl;
					break;
			}

			score += out + 1;
		}
	}

	return score;
}
}        // namespace

uint64_t puzzle_02(std::ifstream &in_file)
{
	return puzzle_02_2(in_file);
}
