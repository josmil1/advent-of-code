#include "Puzzles.hpp"

#include <set>

namespace
{
uint64_t puzzle_01_1(std::ifstream &in_file)
{
	std::string line;

	char dir   = 0;
	uint steps = 0;

	uint64_t count   = 0;
	int      current = 50;        // Initial dial position

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			ss >> dir >> steps;

			// Rotate dial
			if ('L' == dir)
			{
				current -= steps;
			}
			else if ('R' == dir)
			{
				current += steps;
			}
			else
			{
				std::cout << "Error: unknown direction." << std::endl;
			}

			// Calculate position
			current = current % 100;
			if (current < 0)
			{
				current += 100;
			}

			// The actual password is the number of times the dial is
			// left pointing at 0 after any rotation in the sequence
			if (0 == current)
			{
				count++;
			}
		}
	}

	return count;
}

uint64_t puzzle_01_2(std::ifstream &in_file)
{
	std::string line;

	char dir   = 0;
	uint steps = 0;

	uint64_t count   = 0;
	int      current = 50;        // Initial dial position
	int      prev    = current;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			ss >> dir >> steps;

			// Rotate dial
			if ('L' == dir)
			{
				current -= steps;
			}
			else if ('R' == dir)
			{
				current += steps;
			}
			else
			{
				std::cout << "Error: unknown direction." << std::endl;
			}

			// Calculate position
			current = current % 100;
			if (current < 0)
			{
				current += 100;
			}

			// Determine the first step where dial hits zero
			int S = (dir == 'R') ? (100 - prev) % 100 : (prev % 100);
			if (S == 0)
			{
				// One full rotation
				S = 100;
			}

			// Count the number of times any click causes the dial to point at 0,
			// regardless of whether it happens during a rotation or at the end of one.
			uint64_t hits = 0;
			if (steps >= S)
			{
				// First hit + additional full rotations
				hits = 1 + (steps - S) / 100;
			}

			count += hits;

			// Count landing on zero only if we had no zero-hits and we end on zero
			if (current == 0 && hits == 0)
			{
				count++;
			}

			prev = current;
		}
	}

	return count;
}
}        // namespace

uint64_t puzzle_01(std::ifstream &in_file)
{
	return puzzle_01_2(in_file);
}
