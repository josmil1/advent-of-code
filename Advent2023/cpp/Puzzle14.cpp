#include "Puzzles.hpp"

namespace
{
struct Pos
{
	uint32_t x{0};
	uint32_t y{0};
};

struct Rock
{
	Pos  pos;
	bool round;
};

using Grid = std::unordered_map<uint64_t, Rock>;

struct Platform
{
	Grid     grid;
	uint32_t width;
	uint32_t height;
};

inline uint64_t key(uint32_t x, uint32_t y)
{
	return static_cast<uint64_t>(x) << 32 | y;
}

void print(Platform &platform)
{
	for (uint32_t j = 0; j < platform.height; j++)
	{
		for (uint32_t i = 0; i < platform.width; i++)
		{
			if (platform.grid.count(key(i, j)))
			{
				auto &r = platform.grid[key(i, j)];
				if (r.round)
				{
					std::cout << '0';
				}
				else
				{
					std::cout << '#';
				}
			}
			else
			{
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
}

Platform get_platform(std::ifstream &in_file)
{
	Platform platform{};

	std::string line;

	// Parse platform
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == platform.width)
			{
				platform.width = line.size();
			}

			for (uint32_t x = 0; x < platform.width; x++)
			{
				if (line[x] != '.')
				{
					Pos pos{x, platform.height};

					Rock rock;
					rock.pos   = pos;
					rock.round = ('O' == line[x]);

					platform.grid[key(pos.x, pos.y)] = rock;
				}
			}

			platform.height++;
		}
	}

	return platform;
}

Platform tilt_north(Platform &platform)
{
	Platform after_tilt = platform;
	after_tilt.grid.clear();

	for (uint32_t i = 0; i < platform.width; i++)
	{
		uint32_t round_count{0};
		uint32_t square_height{0};
		// Rocks are 0-indexed, but load depends on row count
		for (uint32_t j = 0; j < platform.height; j++)
		{
			if (platform.grid.count(key(i, j)))
			{
				auto &r = platform.grid[key(i, j)];

				Rock new_rock;
				new_rock.round = r.round;

				if (r.round)
				{
					new_rock.pos = Pos{i, round_count + square_height};

					round_count++;
				}
				else
				{
					new_rock.pos = Pos{i, j};

					square_height = j + 1;
					round_count   = 0;
				}

				after_tilt.grid[key(new_rock.pos.x, new_rock.pos.y)] = new_rock;
			}
		}
	}

	return after_tilt;
}

Platform rotate_clockwise(Platform &platform)
{
	Platform after_rotation{};
	after_rotation.width  = platform.height;
	after_rotation.height = platform.width;

	for (uint32_t i = 0; i < platform.width; i++)
	{
		for (uint32_t j = 0; j < platform.height; j++)
		{
			if (platform.grid.count(key(i, j)))
			{
				Pos new_pos{platform.height - 1 - j, i};
				after_rotation.grid[key(new_pos.x, new_pos.y)] = platform.grid[key(i, j)];

				after_rotation.grid[key(new_pos.x, new_pos.y)].pos = new_pos;
			}
		}
	}

	return after_rotation;
}

Platform cycle(Platform &platform)
{
	Platform after_cycle{};

	// Tilt North
	after_cycle = tilt_north(platform);

	// Tilt West
	after_cycle = rotate_clockwise(after_cycle);
	after_cycle = tilt_north(after_cycle);

	// Tilt South
	after_cycle = rotate_clockwise(after_cycle);
	after_cycle = tilt_north(after_cycle);

	// Tilt East
	after_cycle = rotate_clockwise(after_cycle);
	after_cycle = tilt_north(after_cycle);

	after_cycle = rotate_clockwise(after_cycle);

	return after_cycle;
}

uint64_t get_north_beam_load(Platform &platform)
{
	uint64_t load{0};

	for (uint32_t i = 0; i < platform.width; i++)
	{
		for (uint32_t j = 0; j < platform.height; j++)
		{
			if (platform.grid.count(key(i, j)))
			{
				auto &r = platform.grid[key(i, j)];

				if (r.round)
				{
					load += platform.height - j;
				}
			}
		}
	}

	return load;
}

uint64_t puzzle_14_1(std::ifstream &in_file)
{
	auto platform = get_platform(in_file);

	platform = tilt_north(platform);

	return get_north_beam_load(platform);
}

uint64_t puzzle_14_2(std::ifstream &in_file)
{
	auto platform = get_platform(in_file);

	constexpr uint32_t MIN_CYCLES = 100;
	constexpr uint32_t MAX_CYCLES = 1000000000;

	std::unordered_map<uint64_t, uint32_t> load_counts;

	std::vector<uint64_t> load_list;
	load_list.reserve(MIN_CYCLES);

	// Expect the loads to repeat after MIN_CYCLES,
	// at least a few times within as many cycles
	for (uint32_t i = 0; i < MIN_CYCLES * 2; i++)
	{
		platform  = cycle(platform);
		auto load = get_north_beam_load(platform);

		if (i >= MIN_CYCLES)
		{
			load_counts[load]++;
			load_list.push_back(load);
		}
	}

	// Find loop size (length of repeating sequence)
	uint32_t loop_size{0};
	uint32_t min_load_count{std::numeric_limits<uint32_t>::max()};
	for (auto &lc : load_counts)
	{
		min_load_count = std::min(min_load_count, lc.second);
	}
	for (auto &lc : load_counts)
	{
		lc.second = lc.second / min_load_count;
		loop_size += lc.second;
	}

	// Find which load within the repeating sequence will remain after MAX_CYCLES
	uint32_t index = ((MAX_CYCLES - MIN_CYCLES) % loop_size) - 1;

	return load_list[index];
}
}        // namespace

uint64_t puzzle_14(std::ifstream &in_file)
{
	return puzzle_14_2(in_file);
}
