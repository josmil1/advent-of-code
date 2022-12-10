#include "Puzzles.hpp"

namespace
{
using Grid = std::unordered_map<uint64_t, bool>;
inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<uint64_t>(x) << 32 | static_cast<uint32_t>(y);
}

struct Pos
{
	int32_t x{0};
	int32_t y{0};
};

void print(Grid &grid, Pos &max)
{
	for (int32_t j = 0; j <= max.y; j++)
	{
		for (int32_t i = 0; i <= max.x; i++)
		{
			auto grid_key = key(i, j);
			if (grid.count(grid_key) && grid[grid_key])
			{
				std::cout << "#";
			}
			else
			{
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

void new_cycle(uint64_t *cycle, std::vector<int64_t> *strengths, const int64_t X, Grid &grid, Pos *pixel, Pos *max)
{
	// Part 1
	// Interested in the strength during the 20th cycle and every 40 cycles after that
	constexpr uint32_t first{20};
	constexpr uint32_t cadence{40};

	if (first == *cycle ||
	    (*cycle >= cadence && (*cycle - first) % cadence == 0))
	{
		// Interesting signal
		strengths->push_back(X * *cycle);
	}

	// Part 2
	constexpr uint32_t crt_width{40};
	pixel->x = (*cycle - 1) % crt_width;
	pixel->y = (*cycle - 1) / crt_width;

	if (abs(pixel->x - X) <= 1)
	{
		// Lit pixel
		auto grid_key  = key(pixel->x, pixel->y);
		grid[grid_key] = true;
	}
	max->x = std::max(max->x, pixel->x);
	max->y = std::max(max->y, pixel->y);

	*cycle += 1;
}

uint64_t solve(std::ifstream &in_file)
{
	std::string line;

	int64_t  X{1};
	uint64_t cycle{1};

	// Part 1
	std::vector<int64_t> interesting_signal_strengths;

	// Part 2
	Grid grid;
	Pos  pixel, max;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			std::string       command;
			ss >> command;

			if ("noop" == command)
			{
				new_cycle(&cycle, &interesting_signal_strengths, X, grid, &pixel, &max);
			}
			else if ("addx" == command)
			{
				int64_t value;
				ss >> value;

				new_cycle(&cycle, &interesting_signal_strengths, X, grid, &pixel, &max);
				new_cycle(&cycle, &interesting_signal_strengths, X, grid, &pixel, &max);
				X += value;
			}
			else
			{
				std::cout << "Error, unknown command " << command << std::endl;
			}
		}
	}

	// Part 1
	auto strength_sum = std::accumulate(interesting_signal_strengths.begin(), interesting_signal_strengths.end(), 0);

	// Part 2
	print(grid, max);

	return strength_sum;
}
}        // namespace

uint64_t puzzle_10(std::ifstream &in_file)
{
	return solve(in_file);
}
