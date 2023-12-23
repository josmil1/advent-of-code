#include "Puzzles.hpp"

#include <deque>

namespace
{
struct Pos
{
	int32_t x{0};
	int32_t y{0};

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y;
	}
};

enum Direction : uint16_t
{
	Right = 0,
	Down  = 1,
	Left  = 2,
	Up    = 3,
};

struct HeatCell
{
	Pos       pos{};
	int64_t   heat{0};
	Direction dir;
	int32_t   same_direction_count{0};
};

using Grid = std::unordered_map<uint64_t, HeatCell>;

struct HeatMap
{
	Grid    grid;
	int32_t width{0};
	int32_t height{0};
};

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<int64_t>(x) << 32 | y;
}

struct CellState
{
	Pos       pos{};
	Direction dir{};
	int       same_direction_count{0};

	bool operator==(const CellState &other) const
	{
		return pos == other.pos &&
		       dir == other.dir &&
		       same_direction_count == other.same_direction_count;
	}
};

struct CellStateHasher
{
	std::size_t operator()(const CellState &c) const
	{
		using std::hash;
		using std::size_t;
		using std::string;

		return ((hash<string>()(std::to_string(key(c.pos.x, c.pos.y))) ^ (hash<int>()(c.dir) << 1)) >> 1) ^
		       (hash<int>()(c.same_direction_count) << 1);
	}
};

uint64_t get_min_heat(HeatMap &heatmap, uint32_t min_cells = 0, uint32_t max_cells = 0)
{
	int64_t min_heat{std::numeric_limits<int64_t>::max()};

	// Dijkstra's
	std::unordered_map<CellState, uint64_t, CellStateHasher> visited;

	const Pos target_pos = Pos{heatmap.width - 1, heatmap.height - 1};

	std::deque<HeatCell> queue;
	queue.push_back({{0, 0}, 0, Direction::Right, 1});
	queue.push_back({{0, 0}, 0, Direction::Down, 1});

	while (!queue.empty())
	{
		// Pop node with minimum heat
		int64_t  temp_min_heat{std::numeric_limits<int64_t>::max()};
		uint32_t min_index;
		for (int v = 0; v < queue.size(); v++)
		{
			auto node = queue[v];
			if (node.heat < temp_min_heat)
			{
				temp_min_heat = node.heat;
				min_index     = v;
			}
		}
		auto node = queue[min_index];
		queue.erase(queue.begin() + min_index);

		// Track not only distance to this node but also from which direction
		// and momentum (how many steps in same direction)
		if (visited.count({node.pos, node.dir, node.same_direction_count}))
		{
			continue;
		}
		visited[{node.pos, node.dir, node.same_direction_count}] = node.heat;

		// Update the next node in the same direction
		Pos next_pos = node.pos;
		switch (node.dir)
		{
			case Direction::Up:
			{
				next_pos.y--;
				break;
			}
			case Direction::Down:
			{
				next_pos.y++;
				break;
			}
			case Direction::Right:
			{
				next_pos.x++;
				break;
			}
			case Direction::Left:
			{
				next_pos.x--;
				break;
			}
			default:
			{
				std::cout << "Error" << std::endl;
			}
		}

		auto next_min_heat = node.heat + heatmap.grid[key(next_pos.x, next_pos.y)].heat;

		if (next_pos == target_pos)
		{
			if (min_cells == 0 || node.same_direction_count >= min_cells)
			{
				// Reached target with correct number of steps in the same direction
				min_heat = std::min(min_heat, next_min_heat);
				continue;
			}
		}

		// Update neighbors of next node
		std::deque<Direction> next_dirs;

		if (min_cells > 0 && node.same_direction_count < min_cells)
		{
			// Force same direction
			next_dirs.push_back(node.dir);
		}
		else
		{
			// Allow direction change
			next_dirs.push_back(node.dir);

			switch (node.dir)
			{
				case Direction::Up:
				case Direction::Down:
				{
					next_dirs.push_back(Direction::Left);
					next_dirs.push_back(Direction::Right);
					break;
				}
				case Direction::Left:
				case Direction::Right:
				{
					next_dirs.push_back(Direction::Up);
					next_dirs.push_back(Direction::Down);
					break;
				}
				default:
				{
					std::cout << "Error" << std::endl;
				}
			}
		}

		// Force direction change
		if (max_cells > 0 && node.same_direction_count >= max_cells)
		{
			next_dirs.pop_front();
		}

		for (auto &next_dir : next_dirs)
		{
			auto new_next_pos = next_pos;
			switch (next_dir)
			{
				case Direction::Up:
				{
					new_next_pos.y--;
					break;
				}
				case Direction::Down:
				{
					new_next_pos.y++;
					break;
				}
				case Direction::Right:
				{
					new_next_pos.x++;
					break;
				}
				case Direction::Left:
				{
					new_next_pos.x--;
					break;
				}
				default:
				{
					std::cout << "Error" << std::endl;
				}
			}

			// Bounds check
			if (new_next_pos.x >= 0 && new_next_pos.x < heatmap.width &&
			    new_next_pos.y >= 0 && new_next_pos.y < heatmap.height)
			{
				// Valid position to update next
				auto new_same_direction_count = next_dir != node.dir ? 1 : node.same_direction_count + 1;

				queue.push_back({next_pos, next_min_heat, next_dir, new_same_direction_count});
			}
		}
	}

	return min_heat;
}

HeatMap parse_heatmap(std::ifstream &in_file)
{
	HeatMap heatmap{};

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == heatmap.width)
			{
				heatmap.width = line.size();
			}

			for (int32_t i = 0; i < heatmap.width; i++)
			{
				Pos pos{i, static_cast<int32_t>(heatmap.height)};
				heatmap.grid[key(pos.x, pos.y)] = {pos, line[i] - '0'};
			}

			heatmap.height++;
		}
	}

	return heatmap;
}

uint64_t puzzle_17_1(std::ifstream &in_file)
{
	auto heatmap = parse_heatmap(in_file);

	return get_min_heat(heatmap, 0, 3);
}

uint64_t puzzle_17_2(std::ifstream &in_file)
{
	auto heatmap = parse_heatmap(in_file);

	return get_min_heat(heatmap, 4, 10);
}
}        // namespace

uint64_t puzzle_17(std::ifstream &in_file)
{
	return puzzle_17_2(in_file);
}
