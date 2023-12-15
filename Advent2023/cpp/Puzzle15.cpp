#include "Puzzles.hpp"

namespace
{
uint64_t get_hash(std::string str)
{
	uint64_t hash{0};

	for (auto &c : str)
	{
		// Determine the ASCII code for the current character of the string.
		// Increase the current value by the ASCII code you just determined.
		hash += static_cast<int>(c);

		// Set the current value to itself multiplied by 17.
		hash *= 17;

		// Set the current value to the remainder of dividing itself by 256.
		hash = hash % 256;
	}

	return hash;
}

uint64_t puzzle_15_1(std::ifstream &in_file)
{
	uint64_t sum{0};

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			std::string       str;

			while (std::getline(ss, str, ','))
			{
				sum += get_hash(str);
			}
		}
	}

	return sum;
}

uint64_t puzzle_15_2(std::ifstream &in_file)
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

uint64_t puzzle_15(std::ifstream &in_file)
{
	return puzzle_15_1(in_file);
}
