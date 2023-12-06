#include "Puzzles.hpp"

namespace
{
struct Race
{
	uint32_t time;
	uint32_t distance;
};

std::vector<Race> parse_races(std::ifstream &in_file)
{
	std::string line;
	std::string str;
	uint32_t    num;

	std::vector<Race> races;

	// Times
	{
		std::getline(in_file, line);
		std::stringstream ss(line);

		// "Time:"
		ss >> str;

		// x	y	z
		while (ss >> num)
		{
			races.push_back({num});
		}
	}

	// Distances
	{
		std::getline(in_file, line);
		std::stringstream ss(line);

		// "Distance:"
		ss >> str;

		// x	y	z
		uint32_t count{0};
		while (ss >> num)
		{
			races[count].distance = num;
			count++;
		}
	}

	return races;
}

uint64_t get_ways_to_win(Race &race)
{
	uint64_t win_count{0};

	// Boat has a starting speed of 0 millimeters per millisecond.
	// For each millisecond holding down the button,
	// the boat's speed increases by 1 millimeter per millisecond.
	for (uint64_t hold_time = 1ULL; hold_time < race.time; hold_time++)
	{
		uint64_t move_time      = race.time - hold_time;
		uint64_t distance_moved = hold_time * move_time;

		if (distance_moved > race.distance)
		{
			win_count++;
		}
	}

	return win_count;
}

uint64_t puzzle_06_1(std::ifstream &in_file)
{
	auto     races = parse_races(in_file);
	uint64_t ways_to_win_mult{1};

	for (auto &race : races)
	{
		ways_to_win_mult *= get_ways_to_win(race);
	}

	return ways_to_win_mult;
}

uint64_t puzzle_06_2(std::ifstream &in_file)
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

uint64_t puzzle_06(std::ifstream &in_file)
{
	return puzzle_06_1(in_file);
}
