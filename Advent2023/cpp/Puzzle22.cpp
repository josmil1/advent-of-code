#include "Puzzles.hpp"

#include <queue>
#include <set>

namespace
{
bool PART_TWO{false};

struct Pos
{
	int32_t x{0};
	int32_t y{0};
	int32_t z{0};

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y &&
		       z == other.z;
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

// x-y, seen from above (z)
using BrickMap = std::unordered_map<uint64_t, std::vector<Brick *>>;

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<int64_t>(x) << 32 | y;
}

using SupportMap = std::unordered_map<Brick *, std::set<Brick *>>;

void get_supported_by(Brick *brick, std::set<Brick *> &support_list, SupportMap &support_map)
{
	auto &directly_supported = support_map[brick];

	if (directly_supported.size() > 0)
	{
		for (auto &ds : directly_supported)
		{
			if (!support_list.count(ds))
			{
				// Not already in the list, check what it is supported by
				bool all_in_list{true};
				for (auto &s : ds->supported_by)
				{
					all_in_list &= (support_list.count(s) != 0);
				}

				if (all_in_list)
				{
					// This block will fall, all its supports are in the list
					support_list.insert(ds);

					// Check blocks that will fall as a result of this one falling
					get_supported_by(ds, support_list, support_map);
				}
			}
		}
	}
}

int32_t remove_number_before(std::string &str, char sep = ' ')
{
	int32_t number;

	auto pos = str.find(sep);
	number   = std::stol(str.substr(0, pos));
	str      = str.substr(pos + 1);

	return number;
}

uint64_t puzzle_22_1(std::ifstream &in_file)
{
	std::vector<Brick> bricks;

	BrickMap brickmap;

	// Parse bricks
	std::string line;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			Brick brick{};
			brick.a.x = remove_number_before(line, ',');
			brick.a.y = remove_number_before(line, ',');
			brick.a.z = remove_number_before(line, '~');
			brick.b.x = remove_number_before(line, ',');
			brick.b.y = remove_number_before(line, ',');
			brick.b.z = remove_number_before(line);

			if (brick.a.z > brick.b.z)
			{
				// Ensure coordinates are z-sorted
				std::swap(brick.a, brick.b);
			}

			bricks.push_back(brick);
		}
	}

	// Ensure bricks are z-sorted
	std::sort(bricks.begin(), bricks.end());

	// Fill grid and track bricks below other bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		auto &brick = bricks[i];

		auto min_x = std::min(brick.a.x, brick.b.x);
		auto max_x = std::max(brick.a.x, brick.b.x);

		auto min_y = std::min(brick.a.y, brick.b.y);
		auto max_y = std::max(brick.a.y, brick.b.y);

		// If the brick is horizontal, check all existing bricks
		// under each block, and save the highest one
		int32_t highest_z = 0;

		for (auto x = min_x; x <= max_x; x++)
		{
			for (auto y = min_y; y <= max_y; y++)
			{
				if (brickmap.count(key(x, y)))
				{
					auto &supporting_bricks = brickmap[key(x, y)];

					highest_z = std::max(highest_z, supporting_bricks.back()->b.z);
				}
			}
		}

		// Drop brick
		auto height = brick.b.z - brick.a.z;
		brick.a.z   = highest_z + 1;
		brick.b.z   = brick.a.z + height;

		// For each block, check which other brick is directly below it
		for (auto x = min_x; x <= max_x; x++)
		{
			for (auto y = min_y; y <= max_y; y++)
			{
				if (brickmap.count(key(x, y)))
				{
					auto &supporting_bricks = brickmap[key(x, y)];

					if (supporting_bricks.back()->b.z == brick.a.z - 1)
					{
						brick.supported_by.insert(supporting_bricks.back());
					}
				}

				// Add this brick to the map
				brickmap[key(x, y)].push_back(&bricks[i]);
			}
		}
	}

	// Find bricks that are the only support of other bricks
	std::set<Brick *> supporting_bricks;
	for (auto &brick : bricks)
	{
		if (brick.supported_by.size() == 1)
		{
			supporting_bricks.insert(brick.supported_by.begin(), brick.supported_by.end());
		}
	}

	// Part 1: every other bridge can be safely removed
	auto disintegrable_count = bricks.size() - supporting_bricks.size();

	// Part 2: count how many bricks would fall if the supporting bricks were removed

	// Reverse brickmap, to find which bricks are supported by a given brick
	SupportMap support_map;
	for (auto &brick : bricks)
	{
		for (auto &s : brick.supported_by)
		{
			support_map[s].insert(&brick);
		}
	}

	// For each supporting brick, recursively count how many would fall in total
	uint64_t sum{0};
	for (auto &brick : supporting_bricks)
	{
		std::set<Brick *> support_list{brick};
		get_supported_by(brick, support_list, support_map);

		sum += support_list.size() - 1;
	}

	return PART_TWO ? sum : disintegrable_count;
}

uint64_t puzzle_22_2(std::ifstream &in_file)
{
	PART_TWO = true;

	return puzzle_22_1(in_file);
}
}        // namespace

uint64_t puzzle_22(std::ifstream &in_file)
{
	return puzzle_22_2(in_file);
}
