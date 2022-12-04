#include "Puzzles.hpp"

#include <set>

using ItemCount = std::unordered_map<int32_t, int32_t>;

namespace
{
constexpr int32_t lower_min = 'a';
constexpr int32_t lower_max = 'z';
constexpr int32_t upper_min = 'A';

int32_t get_index(char c)
{
	int32_t index = c - lower_min + 1;
	if (index < 0)
	{
		index += lower_max - upper_min + 1;
	}
	return index;
}

int32_t find_duplicate(std::string input)
{
	size_t    compartment_size = input.size() / 2;
	ItemCount count;
	for (size_t i = 0; i < input.size(); i++)
	{
		auto index = get_index(input[i]);

		if (i < compartment_size)
		{
			// First compartment
			count[index]++;
		}
		else
		{
			// Second compartment
			if (count[index] > 0)
			{
				return index;
			}
		}
	}

	std::cout << "Error, no duplicate found";
	return 0;
}

constexpr uint32_t group_size = 3;

int32_t find_badge(ItemCount &count, std::string input, int32_t input_index)
{
	std::unordered_map<int32_t, bool> seen;
	for (size_t i = 0; i < input.size(); i++)
	{
		auto index = get_index(input[i]);

		// Avoid counting items that appear multiple times in the same line
		if (!seen[index])
		{
			count[index]++;
			seen[index] = true;
		}

		if (input_index == group_size - 1 && count[index] == group_size)
		{
			return index;
		}
	}

	return 0;
}

uint64_t puzzle_03_1(std::ifstream &in_file)
{
	std::string line;

	uint64_t priority_sum = 0;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (line.size() % 2 != 0)
			{
				std::cout << "Error, number of items must be even" << std::endl;
			}

			priority_sum += find_duplicate(line);
		}
	}

	return priority_sum;
}

uint64_t puzzle_03_2(std::ifstream &in_file)
{
	std::string line;

	uint64_t  priority_sum = 0;
	uint64_t  input_count  = 0;
	ItemCount count;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			auto badge = find_badge(count, line, input_count);
			if (input_count == group_size - 1)
			{
				// New group
				priority_sum += badge;
				input_count = 0;
				count.clear();
			}
			else
			{
				input_count++;
			}
		}
	}

	return priority_sum;
}
}        // namespace

uint64_t puzzle_03(std::ifstream &in_file)
{
	return puzzle_03_2(in_file);
}
