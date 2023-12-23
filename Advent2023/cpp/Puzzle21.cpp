#include "Puzzles.hpp"

#include <deque>
#include <set>

namespace
{
struct Pos
{
	int64_t x{0};
	int64_t y{0};

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

	bool operator<(const Pos &other) const
	{
		return x == other.x ? y < other.y : x < other.x;
	}
};

using Grid = std::unordered_map<uint64_t, int64_t>;

struct Garden
{
	Grid    grid;
	int64_t width;
	int64_t height;
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

int64_t mod(int64_t a, int64_t b)
{
	// Avoid negative remainder
	return (a % b + b) % b;
}

uint64_t count_positions(Garden &garden, Pos start, uint32_t exact_steps)
{
	std::set<Pos> final_positions;

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

		if (exact_steps == distance)
		{
			final_positions.insert(cur);
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

					if (garden.grid.count(key(mod(new_pos.x, garden.width), mod(new_pos.y, garden.width))))
					{
						// No bounds-check, pattern repeats infinitely
						// Not a rock: visit neighbors
						positions.push_back({new_pos, distance + 1});
					}
				}
			}
		}
	}

	return final_positions.size();
}

uint64_t puzzle_21_1(std::ifstream &in_file)
{
	auto garden = parse_garden(in_file);

	Pos start = {garden.width / 2, garden.height / 2};

	constexpr int STEPS = 6;        // 64 for real input

	return count_positions(garden, start, STEPS);
}

uint64_t fit_and_solve_quadratic(int64_t y_1, int64_t y_2, int64_t y_3, int64_t x = 0)
{
	// Fit quadratic
	// 3 equations, 3 unknowns
	// a*x^2 + b*x + c

	// At x_1 = 0
	int64_t c = y_1;

	// At x_2 = 1
	// a + b + c = y_2
	// a = y_2 - c - b

	// At x_3 = 2
	// 4a + 2b + c = y_3
	// 4 * (y_2 - c - b) + 2b + c = y_3
	// 4y_2 - 4c - 4b + 2b + c = y_3
	// 4y_2 - 3c - 2b = y_3
	// 2b = 4y_2 - 3c - y_3
	int64_t b = ((4 * y_2) - (3 * c) - y_3) / 2;

	int64_t a = y_2 - c - b;

	// Solve quadratic for x
	return a * x * x + b * x + c;
}

uint64_t puzzle_21_2(std::ifstream &in_file)
{
	auto garden = parse_garden(in_file);

	Pos start = {garden.width / 2, garden.height / 2};

	constexpr int STEPS = 26501365;
	// Does not work with example input, requires:
	// -> width = height
	// -> middle row/column must be empty
	int64_t div = STEPS / garden.width;
	int64_t rem = STEPS % garden.width;

	auto y_1 = count_positions(garden, start, rem);
	auto y_2 = count_positions(garden, start, rem + garden.width);
	auto y_3 = count_positions(garden, start, rem + garden.width * 2);

	return fit_and_solve_quadratic(y_1, y_2, y_3, div);
}

}        // namespace

uint64_t puzzle_21(std::ifstream &in_file)
{
	return puzzle_21_2(in_file);
}
