#include "Puzzles.hpp"

#include <set>

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

	bool operator<(const Pos &other) const
	{
		return x == other.x ? y < other.y : x < other.x;
	}

	Pos operator+(const Pos &other) const
	{
		return {x + other.x,
		        y + other.y};
	}
};

const Pos NORTH{0, -1};
const Pos EAST{1, 0};
const Pos SOUTH{0, 1};
const Pos WEST{-1, 0};

struct Step;

struct StepConnection
{
	Step    *step{nullptr};
	uint32_t distance{1};
};

struct Step
{
	Pos                         pos;
	std::vector<Pos>            dirs;
	bool                        visited{false};
	std::vector<StepConnection> connections;
};

using Grid = std::unordered_map<uint64_t, Step>;

struct WalkMap
{
	Grid    grid;
	int32_t width;
	int32_t height;
};

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<int64_t>(x) << 32 | y;
}

inline uint64_t key(Pos pos)
{
	return key(pos.x, pos.y);
}

void print(WalkMap &walkmap, const std::set<Step *> intersections = {})
{
	for (int j = 0; j < walkmap.height; j++)
	{
		for (int i = 0; i < walkmap.width; i++)
		{
			if (walkmap.grid.count(key(i, j)))
			{
				auto step = &walkmap.grid[key(i, j)];
				auto it   = std::find(intersections.begin(), intersections.end(), step);
				if (it != intersections.end())
				{
					std::cout << 'O';
				}
				else
				{
					std::cout << '.';
				}
			}
			else
			{
				std::cout << '#';
			}
		}
		std::cout << std::endl;
	}
}

WalkMap parse_map(std::ifstream &in_file)
{
	WalkMap walkmap{};

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == walkmap.width)
			{
				walkmap.width = line.size();
			}

			for (int32_t x = 0; x < walkmap.width; x++)
			{
				Step step{};
				step.pos = {x, walkmap.height};

				switch (line[x])
				{
					case '.':
					{
						step.dirs.push_back(NORTH);
						step.dirs.push_back(EAST);
						step.dirs.push_back(SOUTH);
						step.dirs.push_back(WEST);

						walkmap.grid[key(step.pos)] = step;
						break;
					}
					case '^':
					{
						step.dirs.push_back(NORTH);

						walkmap.grid[key(step.pos)] = step;
						break;
					}
					case '>':
					{
						step.dirs.push_back(EAST);

						walkmap.grid[key(step.pos)] = step;
						break;
					}
					case 'v':
					{
						step.dirs.push_back(SOUTH);

						walkmap.grid[key(step.pos)] = step;
						break;
					}
					case '<':
					{
						step.dirs.push_back(WEST);

						walkmap.grid[key(step.pos)] = step;
						break;
					}
					case '#':
					{
						// Forest
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}
			}

			walkmap.height++;
		}
	}

	return walkmap;
}

void dfs(WalkMap &walkmap, Step *cur, const Pos end, uint64_t &new_longest_walk, uint64_t &longest_walk)
{
	if (end == cur->pos)
	{
		if (new_longest_walk > longest_walk)
		{
			std::cout << "New longest walk: " << new_longest_walk << std::endl;
		}

		longest_walk = std::max(longest_walk, new_longest_walk);
	}

	for (auto &next : cur->connections)
	{
		if (!next.step->visited)
		{
			new_longest_walk += next.distance;
			next.step->visited = true;

			dfs(walkmap, next.step, end, new_longest_walk, longest_walk);

			next.step->visited = false;
			new_longest_walk -= next.distance;
		}
	}
}

uint64_t get_longest_walk(WalkMap &walkmap, Pos start, Pos end)
{
	std::vector<StepConnection> walk;
	uint64_t                    longest_walk{0};
	uint64_t                    new_longest_walk{0};

	dfs(walkmap, &walkmap.grid[key(start)], end, new_longest_walk, longest_walk);

	return longest_walk;
}

uint64_t puzzle_23_1(std::ifstream &in_file)
{
	auto walkmap = parse_map(in_file);

	Pos start{1, 0};
	Pos end{walkmap.width - 2, walkmap.height - 1};

	// Initialize connections
	for (auto &step : walkmap.grid)
	{
		for (auto &dir : step.second.dirs)
		{
			auto next_pos = step.second.pos + dir;
			if (next_pos.x >= 0 && next_pos.x < walkmap.width &&
			    next_pos.y >= 0 && next_pos.y < walkmap.height &&
			    walkmap.grid.count(key(next_pos)))
			{
				step.second.connections.push_back({&walkmap.grid[key(next_pos)]});
			}
		}
	}

	return get_longest_walk(walkmap, start, end);
}

uint64_t puzzle_23_2(std::ifstream &in_file)
{
	auto walkmap = parse_map(in_file);

	Pos start{1, 0};
	Pos end{walkmap.width - 2, walkmap.height - 1};

	std::set<Step *> intersections{};
	intersections.insert(&walkmap.grid[key(start)]);
	intersections.insert(&walkmap.grid[key(end)]);

	std::vector<Pos> dirs{};
	dirs.push_back(NORTH);
	dirs.push_back(EAST);
	dirs.push_back(SOUTH);
	dirs.push_back(WEST);

	// Prune graph to just a few nodes with
	// fixed distance between them stored
	for (int j = 0; j < walkmap.height; j++)
	{
		for (int i = 0; i < walkmap.width; i++)
		{
			Pos cur{i, j};
			if (walkmap.grid.count(key(cur)))
			{
				auto step = &walkmap.grid[key(cur)];

				// Reset possible directions since now all
				// are allowed (except if blocked by forest)
				step->dirs.clear();
				step->connections.clear();
				for (auto &dir : dirs)
				{
					auto next_pos = cur + dir;
					if (next_pos.x >= 0 && next_pos.x < walkmap.width &&
					    next_pos.y >= 0 && next_pos.y < walkmap.height &&
					    walkmap.grid.count(key(next_pos)))
					{
						step->connections.push_back({&walkmap.grid[key(next_pos)]});
					}
				}

				// Save intersections since these are the only
				// nodes of interest
				if (step->connections.size() > 2)
				{
					intersections.insert(step);
				}
			}
		}
	}

	std::unordered_map<Step *, std::vector<StepConnection>> new_connections;

	// Calculate distance between intersections
	for (auto inter : intersections)
	{
		// Reset visited flags
		for (auto &step : walkmap.grid)
		{
			step.second.visited = false;
		}

		inter->visited = true;

		std::vector<Step *> connecting_walks;
		connecting_walks.push_back(inter);

		uint32_t distance{0};
		while (!connecting_walks.empty())
		{
			// Track distance from interesting (intersection) node
			distance++;

			std::vector<Step *> new_connecting_walks{};

			for (auto cur : connecting_walks)
			{
				for (auto &next : cur->connections)
				{
					if (!next.step->visited)
					{
						next.step->visited = true;

						auto it = std::find(intersections.begin(), intersections.end(), next.step);
						if (it != intersections.end())
						{
							// Arrived at another intersection
							// this connecting walk stops here
							next.distance = distance;

							new_connections[inter].push_back(next);
						}
						else
						{
							// Boring node, keep walking
							new_connecting_walks.push_back(next.step);
						}
					}
				}
			}

			connecting_walks.clear();
			connecting_walks.insert(connecting_walks.begin(), new_connecting_walks.begin(), new_connecting_walks.end());
		}
	}

	for (auto inter : intersections)
	{
		// Replace connections to create a new smaller graph
		inter->connections = new_connections[inter];

		// Reset visited flag for DFS
		inter->visited = false;
	}

	return get_longest_walk(walkmap, start, end);
}
}        // namespace

uint64_t puzzle_23(std::ifstream &in_file)
{
	return puzzle_23_2(in_file);
}
