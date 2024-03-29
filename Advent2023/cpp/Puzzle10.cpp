#include "Puzzles.hpp"

#include <deque>

namespace
{
bool PART_TWO{false};

struct Pos
{
	int32_t x{0};
	int32_t y{0};
};

struct Node
{
	bool north{false};
	bool south{false};
	bool east{false};
	bool west{false};

	Pos     pos;
	int64_t distance;
	bool    visited;
};

using Grid = std::unordered_map<uint64_t, Node>;

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<uint64_t>(x) << 32 | static_cast<uint32_t>(y);
}

inline uint64_t key(Pos &pos)
{
	return key(pos.x, pos.y);
}

uint64_t puzzle_10_1(std::ifstream &in_file)
{
	Grid        grid;
	int32_t     height{0}, width{0};
	std::string line;

	Pos start;

	// Parse map
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			for (int x = 0; x < line.size(); x++)
			{
				Node node{};
				node.pos = {x, height};

				switch (line[x])
				{
					case 'S':        // Start position
					{
						start = node.pos;

						grid[key(node.pos)] = node;
						break;
					}
					case '|':        // A vertical pipe connecting north and south
					{
						node.north = true;
						node.south = true;

						grid[key(node.pos)] = node;
						break;
					}
					case '-':        // A horizontal pipe connecting east and west
					{
						node.east = true;
						node.west = true;

						grid[key(node.pos)] = node;
						break;
					}
					case 'L':        // A 90-degree bend connecting north and east
					{
						node.north = true;
						node.east  = true;

						grid[key(node.pos)] = node;
						break;
					}
					case 'J':        // A 90-degree bend connecting north and west
					{
						node.north = true;
						node.west  = true;

						grid[key(x, height)] = node;
						break;
					}
					case '7':        // A 90-degree bend connecting south and west
					{
						node.south = true;
						node.west  = true;

						grid[key(x, height)] = node;
						break;
					}
					case 'F':        // A 90-degree bend connecting south and east
					{
						node.south = true;
						node.east  = true;

						grid[key(x, height)] = node;
						break;
					}
					case '.':        // Ground; there is no pipe in this tile
					{
						break;
					}
					default:
					{
						std::cout << "Error: " << line[x] << std::endl;
						break;
					}
				}
			}

			height++;
			if (0 == width)
			{
				width = line.size();
			}
		}
	}

	// Determine pipe in start position
	auto &start_node = grid[key(start)];

	if (start.x > 0)
	{
		auto &west_node = grid[key(start.x - 1, start.y)];

		start_node.west = west_node.east;
	}

	if (start.x < width - 1)
	{
		auto &east_node = grid[key(start.x + 1, start.y)];

		start_node.east = east_node.west;
	}

	if (start.y > 0)
	{
		auto &north_node = grid[key(start.x, start.y - 1)];

		start_node.north = north_node.south;
	}

	if (start.y < height - 1)
	{
		auto &south_node = grid[key(start.x, start.y + 1)];

		start_node.south = south_node.north;
	}

	// Part 1: traverse map and record farthest distance from Start
	std::deque<Node *> nodes_to_visit;

	nodes_to_visit.push_back(&start_node);

	int64_t max_distance{0};

	do
	{
		auto &current_node = nodes_to_visit.front();

		if (!current_node->visited)
		{
			current_node->visited = true;

			max_distance = std::max(max_distance, current_node->distance);

			if (current_node->west)
			{
				auto &next_node = grid[key(current_node->pos.x - 1, current_node->pos.y)];

				next_node.distance = current_node->distance + 1;

				nodes_to_visit.push_back(&next_node);
			}
			if (current_node->east)
			{
				auto &next_node = grid[key(current_node->pos.x + 1, current_node->pos.y)];

				next_node.distance = current_node->distance + 1;

				nodes_to_visit.push_back(&next_node);
			}
			if (current_node->north)
			{
				auto &next_node = grid[key(current_node->pos.x, current_node->pos.y - 1)];

				next_node.distance = current_node->distance + 1;

				nodes_to_visit.push_back(&next_node);
			}
			if (current_node->south)
			{
				auto &next_node = grid[key(current_node->pos.x, current_node->pos.y + 1)];

				next_node.distance = current_node->distance + 1;

				nodes_to_visit.push_back(&next_node);
			}
		}

		nodes_to_visit.pop_front();
	} while (!nodes_to_visit.empty());

	// Part 2: count tiles enclosed by the loop
	uint64_t enclosed{0};
	for (uint64_t y = 0; y < height; y++)
	{
		// Any tile that isn't part of the main loop can count as being enclosed by the loop
		// For each such tile, count all loop pipes to its left that have a south (or north) connection
		// Explanation: https://en.wikipedia.org/wiki/Even–odd_rule
		// Visualization: https://imgur.com/a/ukstWKO
		uint64_t south_count{0};
		for (uint64_t x = 0; x < width; x++)
		{
			if (grid.count(key(x, y)) && grid[key(x, y)].visited)
			{
				// Part of the main loop
				if (grid[key(x, y)].south)
				{
					south_count++;
				}
			}
			else
			{
				// Not part of the main loop
				if (south_count % 2 != 0)
				{
					// Number of south connecting pipes to the left is odd
					enclosed++;
				}
			}
		}
	}

	return PART_TWO ? enclosed : max_distance;
}

uint64_t puzzle_10_2(std::ifstream &in_file)
{
	PART_TWO = true;

	return puzzle_10_1(in_file);
}
}        // namespace

uint64_t puzzle_10(std::ifstream &in_file)
{
	return puzzle_10_2(in_file);
}
