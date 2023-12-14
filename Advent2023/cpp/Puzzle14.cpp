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

inline uint64_t key(uint32_t x, uint32_t y)
{
	return static_cast<uint64_t>(x) << 32 | y;
}

uint64_t puzzle_14_1(std::ifstream &in_file)
{
	uint64_t load{0};

	std::string line;

	uint32_t width{0}, height{0};
	Grid     platform;

	// Parse platform
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == width)
			{
				width = line.size();
			}

			for (uint32_t x = 0; x < width; x++)
			{
				if (line[x] != '.')
				{
					Pos pos{x, height};

					Rock rock;
					rock.pos   = pos;
					rock.round = ('O' == line[x]);

					platform[key(pos.x, pos.y)] = rock;
				}
			}

			height++;
		}
	}

	// Print platform
	for (uint32_t j = 0; j < height; j++)
	{
		for (uint32_t i = 0; i < width; i++)
		{
			if (platform.count(key(i, j)))
			{
				auto &r = platform[key(i, j)];
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

	// Tilt north and calculate load
	// Count round rocks from the top until a swuare rock is found
	for (uint32_t i = 0; i < width; i++)
	{
		uint64_t column_load{0};
		uint64_t round_count{0};
		uint64_t square_height{0};
		// Rocks are 0-indexed, but load depends on row count
		for (uint32_t j = 0; j < height; j++)
		{
			if (platform.count(key(i, j)))
			{
				auto &r = platform[key(i, j)];
				if (r.round)
				{
					column_load += (height - square_height - round_count);
					round_count++;
				}
				else
				{
					square_height = j + 1;
					round_count   = 0;
				}
			}
		}

		load += column_load;
	}

	return load;
}

uint64_t puzzle_14_2(std::ifstream &in_file)
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

uint64_t puzzle_14(std::ifstream &in_file)
{
	return puzzle_14_1(in_file);
}
