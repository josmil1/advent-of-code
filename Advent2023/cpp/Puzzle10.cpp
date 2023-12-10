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

	Pos      pos;
	uint64_t start_distance;
	bool     visited;
};

using Grid = std::unordered_map<uint64_t, Node>;

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<uint64_t>(x) << 32 | static_cast<uint32_t>(y);
}

uint64_t puzzle_10_1(std::ifstream &in_file)
{
	Grid        grid;
	uint64_t    line_count{0}, line_width{0};
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
				node.pos.x = x;
				node.pos.y = line_count;

				switch (line[x])
				{
					case 'S':
					{
						start.x = x;
						start.y = line_count;

						grid[key(x, line_count)] = node;
						break;
					}
					case '|':        // A vertical pipe connecting north and south.
					{
						node.north = true;
						node.south = true;

						grid[key(x, line_count)] = node;
						break;
					}
					case '-':        // A horizontal pipe connecting east and west.
					{
						node.east = true;
						node.west = true;

						grid[key(x, line_count)] = node;
						break;
					}
					case 'L':        // A 90-degree bend connecting north and east.
					{
						node.north = true;
						node.east  = true;

						grid[key(x, line_count)] = node;
						break;
					}
					case 'J':        // A 90-degree bend connecting north and west.
					{
						node.north = true;
						node.west  = true;

						grid[key(x, line_count)] = node;
						break;
					}
					case '7':        // A 90-degree bend connecting south and west.
					{
						node.south = true;
						node.west  = true;

						grid[key(x, line_count)] = node;
						break;
					}
					case 'F':        // A 90-degree bend connecting south and east.
					{
						node.south = true;
						node.east  = true;

						grid[key(x, line_count)] = node;
						break;
					}
					case '.':        // Ground; there is no pipe in this tile.
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

			line_count++;
			if (0 == line_width)
			{
				line_width = line.size();
			}
		}
	}

	// Determine pipe in start position
	auto &start_node = grid[key(start.x, start.y)];

	if (start.x > 0)
	{
		auto &west_node = grid[key(start.x - 1, start.y)];

		start_node.west = west_node.east;
	}

	if (start.x < line_width - 1)
	{
		auto &east_node = grid[key(start.x + 1, start.y)];

		start_node.east = east_node.west;
	}

	if (start.y > 0)
	{
		auto &north_node = grid[key(start.x, start.y - 1)];

		start_node.north = north_node.south;
	}

	if (start.y < line_count - 1)
	{
		auto &south_node = grid[key(start.x, start.y + 1)];

		start_node.south = south_node.north;
	}

	// Traverse map and record farthest distance from Start
	struct NodeTracker
	{
		Pos      pos;
		uint64_t distance;
	};
	std::deque<NodeTracker> nodes_to_visit;

	if (start_node.west)
	{
		nodes_to_visit.push_back({{start.x - 1, start.y}, 0});
	}
	if (start_node.east)
	{
		nodes_to_visit.push_back({{start.x + 1, start.y}, 0});
	}
	if (start_node.north)
	{
		nodes_to_visit.push_back({{start.x, start.y - 1}, 0});
	}
	if (start_node.south)
	{
		nodes_to_visit.push_back({{start.x, start.y + 1}, 0});
	}

	start_node.visited = true;

	uint64_t max_distance{0};
	while (true)
	{
		auto node_tracker = nodes_to_visit.front();
		nodes_to_visit.pop_front();

		auto &node = grid[key(node_tracker.pos.x, node_tracker.pos.y)];

		if (!node.visited)
		{
			node.start_distance = node_tracker.distance + 1;

			max_distance = std::max(max_distance, node.start_distance);

			// std::cout << "(" << node.pos.x << ", " << node.pos.y << "): " << node.start_distance << std::endl;

			if (node.west)
			{
				nodes_to_visit.push_back({{node.pos.x - 1, node.pos.y}, node.start_distance});
				// std::cout << "go west ";
			}
			if (node.east)
			{
				nodes_to_visit.push_back({{node.pos.x + 1, node.pos.y}, node.start_distance});
				// std::cout << "go east ";
			}
			if (node.north)
			{
				nodes_to_visit.push_back({{node.pos.x, node.pos.y - 1}, node.start_distance});
				// std::cout << "go north ";
			}
			if (node.south)
			{
				nodes_to_visit.push_back({{node.pos.x, node.pos.y + 1}, node.start_distance});
				// std::cout << "go south ";
			}

			// std::cout << std::endl;

			node.visited = true;
		}

		if (nodes_to_visit.empty())
		{
			break;
		}
	}

	uint64_t inside{0};
	for (uint64_t y = 0; y < line_count; y++)
	{
		uint64_t south_count{0};
		for (uint64_t x = 0; x < line_width; x++)
		{
			// Any tile that isn't part of the main loop can count as being enclosed by the loop.
			if (grid.count(key(x, y)))
			{
				auto &node = grid[key(x, y)];

				if (node.visited && node.south)
				{
					south_count++;
				}
				else if (!node.visited && south_count % 2 != 0)
				{
					inside++;
				}
			}
			else
			{
				if (south_count % 2 != 0)
				{
					inside++;
				}
			}
		}
	}

	return PART_TWO ? inside : max_distance;
}

uint64_t puzzle_10_2(std::ifstream &in_file)
{
	PART_TWO = true;

	return puzzle_10_1(in_file);
}
}        // namespace

uint64_t puzzle_10(std::ifstream &in_file)
{
	return puzzle_10_1(in_file);
}
