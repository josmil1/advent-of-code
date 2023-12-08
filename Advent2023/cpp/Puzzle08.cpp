#include "Puzzles.hpp"

#include <numeric>

// Lowest common multiple functions from https://stackoverflow.com/a/4229930
namespace
{
uint64_t gcd(uint64_t a, uint64_t b)
{
	while (true)
	{
		if (0 == a)
		{
			return b;
		}

		b %= a;

		if (0 == b)
		{
			return a;
		}

		a %= b;
	}
}

uint64_t lcm(uint64_t a, uint64_t b)
{
	uint64_t temp = gcd(a, b);

	return temp ? (a / temp * b) : 0;
}

uint64_t lcm_vector(std::vector<uint64_t> v)
{
	uint64_t result = std::accumulate(v.begin(), v.end(), 1ULL, lcm);

	return result;
}

struct Location
{
	std::string left;
	std::string right;
};

using LocationMap = std::unordered_map<std::string, Location>;

std::vector<std::string> start_locations;

LocationMap parse_map(std::ifstream &in_file)
{
	LocationMap locations;

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::string       start, left, right, bin;
			std::stringstream ss(line);

			// start = (left, right)
			ss >> start >> bin >> left >> right;

			locations[start] = {left.substr(1, 3), right.substr(0, 3)};

			// For Part 2 only
			if ('A' == start[2])
			{
				start_locations.push_back(start);
			}
		}
	}

	return locations;
}

uint64_t puzzle_08_1(std::ifstream &in_file)
{
	std::string line;

	std::string sequence;

	std::getline(in_file, sequence);

	auto locations = parse_map(in_file);

	const std::string origin = "AAA";
	const std::string goal   = "ZZZ";

	std::string current    = origin;
	uint64_t    step_index = 0;

	while (current != goal)
	{
		auto current_index = step_index % sequence.size();
		char current_step  = sequence[current_index];

		auto current_options = locations[current];

		if ('R' == current_step)
		{
			current = current_options.right;
		}
		else
		{
			current = current_options.left;
		}

		step_index++;
	}

	return step_index;
}

uint64_t puzzle_08_2(std::ifstream &in_file)
{
	std::string line;

	std::string sequence;

	std::getline(in_file, sequence);

	auto locations = parse_map(in_file);

	std::vector<uint64_t> steps(start_locations.size(), 0);

	for (uint64_t i = 0; i < start_locations.size(); i++)
	{
		uint64_t step_index = 0;

		while ('Z' != start_locations[i][2])
		{
			auto current_index = step_index % sequence.size();
			char current_step  = sequence[current_index];

			auto current_options = locations[start_locations[i]];

			if ('R' == current_step)
			{
				start_locations[i] = current_options.right;
			}
			else
			{
				start_locations[i] = current_options.left;
			}

			step_index++;
		}

		steps[i] = step_index;
	}

	return lcm_vector(steps);
}
}        // namespace

uint64_t puzzle_08(std::ifstream &in_file)
{
	return puzzle_08_2(in_file);
}
