#include "Puzzles.hpp"

namespace
{
struct Pos
{
	int32_t x{0};
	int32_t y{0};

	bool     visited{false};
	uint32_t elevation{0};
	uint32_t shortest_distance{std::numeric_limits<uint32_t>::max()};
};

using Grid = std::unordered_map<uint64_t, Pos>;
inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<uint64_t>(x) << 32 | static_cast<uint64_t>(y);
}

void print(Grid &grid, uint32_t max_x, uint32_t max_y)
{
	for (int j = 0; j <= max_y; j++)
	{
		for (int i = 0; i <= max_x; i++)
		{
			auto &node = grid[key(i, j)];
			if (node.visited)
			{
				std::cout << grid[key(i, j)].shortest_distance;
			}
			else
			{
				std::cout << ".";
			}
			std::cout << char(grid[key(i, j)].elevation + 'a') << "\t";
		}
		std::cout << std::endl;
	}
}

Grid get_grid(std::ifstream &in_file, uint64_t *start_key, uint64_t *end_key)
{
	Grid grid;

	std::string line;
	size_t      grid_width{0};
	char        min_elevation{'a'}, max_elevation{'z'};
	int32_t     x{0}, y{0};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			x = 0;

			if (0 == grid_width)
			{
				grid_width = line.size();
			}

			for (auto c : line)
			{
				Pos new_pos{};

				if ('E' == c)
				{
					c        = max_elevation;
					*end_key = key(x, y);
				}
				else if ('S' == c)
				{
					c          = min_elevation;
					*start_key = key(x, y);
				}
				new_pos.x         = x;
				new_pos.y         = y;
				new_pos.elevation = c - min_elevation;
				grid[key(x, y)]   = std::move(new_pos);
				x++;
			}
			y++;
		}
	}

	return grid;
}

uint64_t min_distance(std::vector<uint64_t> *keys_to_visit, Grid &grid)
{
	// Return key for connection with shortest distance
	uint32_t min_distance = std::numeric_limits<uint32_t>::max();
	size_t   min_distance_it{0};
	for (size_t i = 0; i < keys_to_visit->size(); i++)
	{
		auto distance = grid[keys_to_visit->at(i)].shortest_distance;
		if (distance <= min_distance)
		{
			min_distance    = distance;
			min_distance_it = i;
		}
	}

	uint64_t min_distance_key = keys_to_visit->at(min_distance_it);

	// Remove from list as it is about to be processed
	keys_to_visit->erase(keys_to_visit->begin() + min_distance_it);

	return min_distance_key;
}

void dijkstra(Grid &grid, uint64_t start_key, bool part_1)
{
	int32_t max_x{0}, max_y{0};
	grid[start_key].shortest_distance = 0;

	// Dijkstra’s Algorithm
	std::vector<uint64_t> keys_to_visit;
	keys_to_visit.push_back(start_key);
	size_t keys_to_visit_count = keys_to_visit.size();
	while (keys_to_visit_count > 0)
	{
		auto  next_key = min_distance(&keys_to_visit, grid);
		auto &pos      = grid[next_key];
		pos.visited    = true;

		// Process neighbors
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 || j == 0)
				{
					// Orthogonally adjacent
					auto neighbor_key = key(pos.x + i, pos.y + j);
					if (grid.count(neighbor_key) > 0)
					{
						auto &neighbor = grid[neighbor_key];
						auto  can_move = part_1 ? (neighbor.elevation < pos.elevation || neighbor.elevation - pos.elevation <= 1) :
						                          (neighbor.elevation > pos.elevation || pos.elevation - neighbor.elevation <= 1);
						if (!neighbor.visited && can_move)
						{
							neighbor.shortest_distance = std::min(neighbor.shortest_distance, pos.shortest_distance + 1);
							keys_to_visit.push_back(neighbor_key);

							max_x = std::max(max_x, neighbor.x);
							max_y = std::max(max_y, neighbor.y);
						}
					}
				}
			}
		}

		// Remove visited nodes
		auto it = keys_to_visit.begin();
		while (it != keys_to_visit.end())
		{
			if (grid[*it].visited)
			{
				it = keys_to_visit.erase(it);
			}
			else
			{
				++it;
			}
		}

		keys_to_visit_count = keys_to_visit.size();
	}

	print(grid, max_x, max_y);
}

uint64_t puzzle_12_1(std::ifstream &in_file)
{
	uint64_t start_key, end_key;
	Grid     grid = get_grid(in_file, &start_key, &end_key);

	dijkstra(grid, start_key, true);

	return grid[end_key].shortest_distance;
}

uint64_t puzzle_12_2(std::ifstream &in_file)
{
	uint64_t start_key, end_key;
	Grid     grid = get_grid(in_file, &start_key, &end_key);

	// Run backwards, from end to all possible start positions
	grid[start_key].shortest_distance = std::numeric_limits<uint32_t>::max();
	start_key                         = end_key;

	dijkstra(grid, start_key, false);

	// Find closest start position
	uint32_t min_distance = std::numeric_limits<uint32_t>::max();
	for (auto &it : grid)
	{
		if (it.second.elevation == 0 && it.second.shortest_distance < min_distance)
		{
			min_distance = it.second.shortest_distance;
		}
	}

	return min_distance;
}
}        // namespace

uint64_t puzzle_12(std::ifstream &in_file)
{
	return puzzle_12_1(in_file);
}
