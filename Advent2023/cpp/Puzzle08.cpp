#include "Puzzles.hpp"

namespace
{
struct Location
{
	std::string left;
	std::string right;
};

uint64_t puzzle_08_1(std::ifstream &in_file)
{
	std::string line;

	std::string                               sequence;
	std::unordered_map<std::string, Location> locations;

	std::getline(in_file, sequence);

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::string       start, left, right, bin;
			std::stringstream ss(line);

			// start = (left, right)
			ss >> start >> bin >> left >> right;

			locations[start] = {left.substr(1, 3), right.substr(0, 3)};
		}
	}

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

uint64_t puzzle_08(std::ifstream &in_file)
{
	return puzzle_08_1(in_file);
}
