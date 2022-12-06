#include "Puzzles.hpp"

#include <set>

namespace
{
constexpr uint32_t MARKER_SIZE  = 4;
constexpr uint32_t MESSAGE_SIZE = 14;

uint64_t puzzle_06_1(std::ifstream &in_file)
{
	std::string line;

	while (std::getline(in_file, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			if (i >= MARKER_SIZE)
			{
				std::set<char> unique_chars;

				for (int m = 0; m < MARKER_SIZE; m++)
				{
					unique_chars.insert(line[i - m]);
				}

				if (unique_chars.size() == MARKER_SIZE)
				{
					return i + 1;
				}
			}
		}
	}

	return 0;
}

uint64_t puzzle_06_2(std::ifstream &in_file)
{
	std::string line;

	while (std::getline(in_file, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			if (i >= MESSAGE_SIZE)
			{
				std::set<char> unique_chars;

				for (int m = 0; m < MESSAGE_SIZE; m++)
				{
					unique_chars.insert(line[i - m]);
				}

				if (unique_chars.size() == MESSAGE_SIZE)
				{
					return i + 1;
				}
			}
		}
	}

	return 0;
}
}        // namespace

uint64_t puzzle_06(std::ifstream &in_file)
{
	return puzzle_06_2(in_file);
}
