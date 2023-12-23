#include "Puzzles.hpp"

#include <stack>

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

	Pos operator+(const Pos &other) const
	{
		return {x + other.x,
		        y + other.y};
	}
};

enum Direction
{
	North = 0,
	East  = 1,
	South = 2,
	West  = 3,
};

struct Step
{
	std::vector<Pos> dirs;
	bool             visited{false};
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

void print(WalkMap &walkmap)
{
	for (int j = 0; j < walkmap.height; j++)
	{
		for (int i = 0; i < walkmap.width; i++)
		{
			if (walkmap.grid.count(key(i, j)))
			{
				std::cout << '.';
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

	Pos north{0, -1};
	Pos east{1, 0};
	Pos south{0, 1};
	Pos west{-1, 0};

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
				Pos  pos{x, walkmap.height};

				switch (line[x])
				{
					case '.':
					{
						step.dirs.push_back(north);
						step.dirs.push_back(east);
						step.dirs.push_back(south);
						step.dirs.push_back(west);

						walkmap.grid[key(pos)] = step;
						break;
					}
					case '^':
					{
						step.dirs.push_back(north);

						walkmap.grid[key(pos)] = step;
						break;
					}
					case '>':
					{
						step.dirs.push_back(east);

						walkmap.grid[key(pos)] = step;
						break;
					}
					case 'v':
					{
						step.dirs.push_back(south);

						walkmap.grid[key(pos)] = step;
						break;
					}
					case '<':
					{
						step.dirs.push_back(west);

						walkmap.grid[key(pos)] = step;
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

struct StackElement
{
	Pos      pos{};
	uint32_t distance{0};
};

void dfs(WalkMap &walkmap, Pos cur, const Pos end, std::vector<Pos> &path, uint64_t &longest_path)
{
	if (end == cur)
	{
		longest_path = std::max(longest_path, static_cast<uint64_t>(path.size()));
	}

	auto &step = walkmap.grid[key(cur)];

	for (auto dir : step.dirs)
	{
		auto next_pos = cur + dir;

		if (next_pos.x >= 0 && next_pos.x < walkmap.width &&
		    next_pos.y >= 0 && next_pos.y < walkmap.height &&
		    walkmap.grid.count(key(next_pos)) &&
		    !walkmap.grid[key(next_pos)].visited)
		{
			auto &next_step = walkmap.grid[key(next_pos)];

			path.push_back(next_pos);
			next_step.visited = true;
			dfs(walkmap, next_pos, end, path, longest_path);
			next_step.visited = false;
			path.pop_back();
		}
	}
}

uint64_t get_longest_path(WalkMap &walkmap, Pos start, Pos end)
{
	std::vector<Pos> path;
	uint64_t         longest_path{0};

	dfs(walkmap, start, end, path, longest_path);

	return longest_path;
}

uint64_t puzzle_23_1(std::ifstream &in_file)
{
	auto walkmap = parse_map(in_file);

	Pos start{1, 0};
	Pos end{walkmap.width - 2, walkmap.height - 1};

	return get_longest_path(walkmap, start, end);
}

uint64_t puzzle_23_2(std::ifstream &in_file)
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

uint64_t puzzle_23(std::ifstream &in_file)
{
	return puzzle_23_1(in_file);
}
