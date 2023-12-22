#include "Puzzles.hpp"

#include <set>

namespace
{
struct Pos
{
	int32_t x{0};
	int32_t y{0};
	int32_t z{0};

	Pos operator+(const Pos &other) const
	{
		return {x + other.x,
		        y + other.y,
		        z + other.z};
	}

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y &&
		       z == other.z;
	}

	bool operator<(const Pos &other) const
	{
		return z == other.z ? (x == other.x ? y < other.y : x < other.x) : z < other.z;
	}
};

struct Brick
{
	Pos a;
	Pos b;

	std::set<Brick *> supported_by;

	bool operator==(const Brick &other) const
	{
		return a == other.a &&
		       b == other.b;
	}

	bool operator<(const Brick &other) const
	{
		return b.z < other.b.z;
	}
};

using Grid = std::unordered_map<uint64_t, std::vector<Brick *>>;

// x-y, seen from above (z)
struct BrickMap
{
	Grid    grid;
	int32_t width;
	int32_t height;
};

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<int64_t>(x) << 32 | y;
}

inline uint64_t key(Pos &pos)
{
	return key(pos.x, pos.y);
}

uint64_t puzzle_22_1(std::ifstream &in_file)
{
	std::vector<Brick> bricks;

	BrickMap brickmap;

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			Brick brick{};
			auto  sep = line.find(',');
			brick.a.x = std::stol(line.substr(0, sep));
			line      = line.substr(sep + 1);

			sep       = line.find(',');
			brick.a.y = std::stol(line.substr(0, sep));
			line      = line.substr(sep + 1);

			sep       = line.find('~');
			brick.a.z = std::stol(line.substr(0, sep));
			line      = line.substr(sep + 1);

			sep       = line.find(',');
			brick.b.x = std::stol(line.substr(0, sep));
			line      = line.substr(sep + 1);

			sep       = line.find(',');
			brick.b.y = std::stol(line.substr(0, sep));
			line      = line.substr(sep + 1);

			sep       = line.find(',');
			brick.b.z = std::stol(line.substr(0, sep));
			line      = line.substr(sep + 1);

			if (brick.a.z > brick.b.z)
			{
				std::swap(brick.a, brick.b);
			}

			bricks.push_back(brick);
		}
	}

	// Sort by Z
	std::sort(bricks.begin(), bricks.end());

	// Fill grid and track bricks below other bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		auto &brick = bricks[i];

		auto min_x = std::min(brick.a.x, brick.b.x);
		auto max_x = std::max(brick.a.x, brick.b.x);

		auto min_y = std::min(brick.a.y, brick.b.y);
		auto max_y = std::max(brick.a.y, brick.b.y);

		int32_t highest_z = 0;

		for (auto x = min_x; x <= max_x; x++)
		{
			for (auto y = min_y; y <= max_y; y++)
			{
				if (brickmap.grid.count(key(x, y)))
				{
					auto &supporting_bricks = brickmap.grid[key(x, y)];

					highest_z = std::max(highest_z, supporting_bricks.back()->b.z);

					/*
					auto below_bricks = supporting_bricks;
					auto prev_z       = below_bricks.back()->b.z;
					do
					{
					    if (prev_z == supporting_bricks.back()->b.z ||
					        prev_z == supporting_bricks.back()->b.z - 1)
					    {
					        brick.supported_by.insert(below_bricks.back());

					        prev_z = supporting_bricks.back()->b.z;
					    }
					    else
					    {
					        break;
					    }
					    below_bricks.pop_back();
					} while (!below_bricks.empty());

					brick.supported_by.insert(below_bricks.begin(), below_bricks.end());
					*/
				}
			}
		}

		std::cout << "before drop: a.z = " << brick.a.z << " b.z = " << brick.b.z << std::endl;
		// Drop brick
		auto height = brick.b.z - brick.a.z;
		brick.a.z   = highest_z + 1;
		brick.b.z   = brick.a.z + height;
		std::cout << "after drop: a.z = " << brick.a.z << " b.z = " << brick.b.z << std::endl;

		for (auto x = min_x; x <= max_x; x++)
		{
			for (auto y = min_y; y <= max_y; y++)
			{
				if (brickmap.grid.count(key(x, y)))
				{
					auto &supporting_bricks = brickmap.grid[key(x, y)];

					std::cout << "this brick.a.z " << brick.a.z << " vs " << supporting_bricks.back()->b.z << std::endl;

					if (supporting_bricks.back()->b.z == brick.a.z - 1)
					{
						brick.supported_by.insert(supporting_bricks.back());
					}
				}

				brickmap.grid[key(x, y)].push_back(&bricks[i]);
			}
		}

		std::cout << "supported by " << brick.supported_by.size() << std::endl;
	}

	std::set<Brick *> disintegrable_bricks;

	std::set<Brick *> supporting_bricks;

	for (auto &brick : bricks)
	{
		disintegrable_bricks.insert(&brick);
		if (brick.supported_by.size() == 1)
		{
			supporting_bricks.insert(brick.supported_by.begin(), brick.supported_by.end());
		}
	}

	int disintegrable_count{0};
	for (auto d : disintegrable_bricks)
	{
		if (!supporting_bricks.count(d))
		{
			disintegrable_count++;
		}
	}

	return disintegrable_count;
}

uint64_t puzzle_22_2(std::ifstream &in_file)
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

uint64_t puzzle_22(std::ifstream &in_file)
{
	return puzzle_22_1(in_file);
}
