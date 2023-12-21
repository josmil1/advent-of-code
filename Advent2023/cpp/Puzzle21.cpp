#include "Puzzles.hpp"

#include <deque>

namespace
{
struct Pos
{
	int32_t x{0};
	int32_t y{0};

	Pos operator+(const Pos &other) const
	{
		return {x + other.x,
		        y + other.y};
	}

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y;
	}
};

using Grid = std::unordered_map<uint64_t, uint32_t>;

struct Garden
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

struct PlotState
{
	Pos pos{};
	int distance{0};

	bool operator==(const PlotState &other) const
	{
		return pos == other.pos &&
		       distance == other.distance;
	}
};

struct PlotStateHasher
{
	std::size_t operator()(const PlotState &c) const
	{
		using std::hash;
		using std::size_t;
		using std::string;

		return ((hash<string>()(std::to_string(key(c.pos.x, c.pos.y))) ^ (hash<int>()(c.distance) << 1)) >> 1);
	}
};

Garden parse_garden(std::ifstream &in_file)
{
	Garden garden{};

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == garden.width)
			{
				garden.width = line.size();
			}

			for (int i = 0; i < garden.width; i++)
			{
				if ('#' != line[i])
				{
					garden.grid[key(i, garden.height)] = 0;
				}
			}

			garden.height++;
		}
	}

	return garden;
}

void print(Garden &garden, std::vector<Pos> *final_positions = nullptr)
{
	for (int j = 0; j < garden.height; j++)
	{
		for (int i = 0; i < garden.width; i++)
		{
			if (garden.grid.count(key(i, j)))
			{
				if (final_positions && std::find(final_positions->begin(), final_positions->end(), Pos{i, j}) != final_positions->end())
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

uint64_t puzzle_21_1(std::ifstream &in_file)
{
	auto garden = parse_garden(in_file);

	Pos start = {garden.width / 2, garden.height / 2};

	std::vector<Pos> final_positions;

	constexpr int STEPS = 64;

	// BFS
	std::unordered_map<PlotState, bool, PlotStateHasher> visited;

	std::deque<PlotState> positions;
	positions.push_back({start, 0});

	while (!positions.empty())
	{
		auto cur      = positions.front().pos;
		auto distance = positions.front().distance;
		positions.pop_front();

		if (visited.count({cur, distance}) && visited[{cur, distance}])
		{
			continue;
		}

		visited[{cur, distance}] = true;

		if (STEPS == distance)
		{
			final_positions.push_back(cur);
		}
		else
		{
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if ((i == 0 && j == 0) || (std::abs(i) == std::abs(j)))
					{
						// Orthogonal only, and not-self
						continue;
					}

					auto new_pos = cur + Pos{i, j};

					if (new_pos.x >= 0 && new_pos.x < garden.width &&
					    new_pos.y >= 0 && new_pos.y < garden.height &&
					    garden.grid.count(key(new_pos)))
					{
						// Within bounds, and not a rock: visit neighbors
						positions.push_back({new_pos, distance + 1});
					}
				}
			}
		}
	}

	return final_positions.size();
}

uint64_t puzzle_21_2(std::ifstream &in_file)
{
	auto garden = parse_garden(in_file);

	Pos start = {garden.width / 2, garden.height / 2};

	std::cout << garden.width << " x " << garden.height << std::endl;

	return 0;
}
}        // namespace

uint64_t puzzle_21(std::ifstream &in_file)
{
	return puzzle_21_1(in_file);
}
