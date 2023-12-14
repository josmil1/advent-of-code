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

uint64_t puzzle_14_1(std::ifstream &in_file)
{
	uint64_t load{0};

	auto platform = get_platform(in_file);

	// Tilt north and calculate load
	// Count round rocks from the top until a square rock is found
	Platform after_north_tilt = platform;
	after_north_tilt.grid.clear();
	for (uint32_t i = 0; i < platform.width; i++)
	{
		uint64_t column_load{0};
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

					column_load += (platform.height - square_height - round_count);
					round_count++;
				}
				else
				{
					new_rock.pos = Pos{i, j};

					square_height = j + 1;
					round_count   = 0;
				}

				after_north_tilt.grid[key(new_rock.pos.x, new_rock.pos.y)] = new_rock;
			}
		}

		load += column_load;
	}

	print(after_north_tilt);

	// Rotate clockwise, then tilt north
	Platform after_west_tilt;
	for (uint32_t i = 0; i < after_north_tilt.width; i++)
	{
		for (uint32_t j = 0; j < after_north_tilt.height; j++)
		{
			if (after_north_tilt.grid.count(key(i, j)))
			{
				Pos new_pos{after_north_tilt.height - 1 - j, i};
				after_west_tilt.grid[key(new_pos.x, new_pos.y)] = after_north_tilt.grid[key(i, j)];

				after_west_tilt.grid[key(new_pos.x, new_pos.y)].pos = new_pos;
			}
		}
	}

	std::cout << "------------------" << std::endl;
	after_west_tilt.width  = after_north_tilt.height;
	after_west_tilt.height = after_north_tilt.width;
	print(after_west_tilt);

	/*
	Grid after_west_tilt;
	platform = after_north_tilt;
	for (uint32_t j = 0; j < height; j++)
	{
	    uint32_t round_count{0};
	    uint32_t square_height{0};
	    for (uint32_t i = 0; i < width; i++)
	    {
	        if (platform.count(key(i, j)))
	        {
	            auto &r = platform[key(i, j)];

	            Rock new_rock;
	            new_rock.round = r.round;

	            if (r.round)
	            {
	                new_rock.pos = Pos{round_count + square_height, j};

	                round_count++;
	            }
	            else
	            {
	                new_rock.pos = Pos{i, j};

	                square_height = i + 1;
	                round_count   = 0;
	            }

	            after_west_tilt[key(new_rock.pos.x, new_rock.pos.y)] = new_rock;
	        }
	    }
	}

	std::cout << "------------------" << std::endl;
	print(after_west_tilt, width, height);

	Grid after_south_tilt;
	platform = after_west_tilt;
	for (uint32_t i = 0; i < width; i++)
	{
	    uint32_t round_count{0};
	    uint32_t square_height{0};
	    for (uint32_t j = 0; j < height; j++)
	    {
	        if (platform.count(key(width - 1 - i, height - 1 - j)))
	        {
	            auto &r = platform[key(width - 1 - i, height - 1 - j)];

	            Rock new_rock;
	            new_rock.round = r.round;

	            if (r.round)
	            {
	                new_rock.pos = Pos{width - 1 - i, height - 1 - (round_count + square_height)};

	                round_count++;
	            }
	            else
	            {
	                new_rock.pos = Pos{width - 1 - i, height - 1 - j};

	                square_height = height - 1 - (j + 1);
	                round_count   = 0;
	            }

	            after_south_tilt[key(new_rock.pos.x, new_rock.pos.y)] = new_rock;
	        }
	    }
	}

	std::cout << "------------------" << std::endl;
	print(after_south_tilt, width, height);
	*/

	return load;
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

uint64_t puzzle_14_2(std::ifstream &in_file)
{
	auto platform = get_platform(in_file);

	for (uint32_t i = 0; i < 1000000000; i++)
	{
		platform = cycle(platform);
	}
	print(platform);

	/*
	print(platform);

	platform = tilt_north(platform);

	std::cout << "------------------" << std::endl;
	print(platform);

	platform = rotate_clockwise(platform);

	std::cout << "------------------" << std::endl;
	print(platform);

	platform = tilt_north(platform);

	std::cout << "------------------" << std::endl;
	print(platform);
	*/

	return get_north_beam_load(platform);
}
}        // namespace

uint64_t puzzle_14(std::ifstream &in_file)
{
	return puzzle_14_2(in_file);
}
