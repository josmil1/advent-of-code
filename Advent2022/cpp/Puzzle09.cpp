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

uint32_t print(Grid &grid, Pos min, Pos max, Pos cur_head, Pos cur_tail)
{
	// Print from top to bottom
	uint32_t visited = 0;
	for (int32_t j = max.y; j >= min.y; j--)
	{
		std::cout << j << " ";
		for (int32_t i = min.x; i <= max.x; i++)
		{
			auto grid_key = key(i, j);
			if (grid.count(grid_key) && grid[grid_key])
			{
				visited++;
			}

			if (i == cur_tail.x && j == cur_tail.y)
			{
				std::cout << "T";
			}
			else if (i == cur_head.x && j == cur_head.y)
			{
				std::cout << "H";
			}
			else
			{
				if (grid.count(grid_key) && grid[grid_key])
				{
					std::cout << "#";
				}
				else
				{
					std::cout << ".";
				}
			}
		}
		std::cout << std::endl;
	}

	return visited;
}

int32_t close_gap(int32_t from, int32_t to)
{
	return (from > to) ? from - 1 : from + 1;
}

uint64_t puzzle_09_1(std::ifstream &in_file)
{
	std::string line;

	Grid grid;
	Pos  head, tail, min, max;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			char              command;
			uint32_t          steps;
			ss >> command >> steps;

			for (uint32_t s = 0; s < steps; s++)
			{
				switch (command)
				{
					case 'U':
					{
						// Move up
						head.y++;
						break;
					}
					case 'D':
					{
						// Move down
						head.y--;
						break;
					}
					case 'L':
					{
						// Move left
						head.x--;
						break;
					}
					case 'R':
					{
						// Move right
						head.x++;
						break;
					}
					default:
					{
						std::cout << "Error, unknown command " << command << std::endl;
						break;
					}
				}

				bool too_far_x        = abs(head.x - tail.x) > 1;
				bool too_far_y        = abs(head.y - tail.y) > 1;
				bool diff_row_and_col = head.x != tail.x && head.y != tail.y;
				if (too_far_x || too_far_y)
				{
					if (too_far_x || diff_row_and_col)
					{
						tail.x = close_gap(tail.x, head.x);
					}

					if (too_far_y || diff_row_and_col)
					{
						tail.y = close_gap(tail.y, head.y);
					}
				}

				auto grid_key  = key(tail.x, tail.y);
				grid[grid_key] = true;

				max.x = std::max(max.x, head.x);
				max.y = std::max(max.y, head.y);
				min.x = std::min(min.x, head.x);
				min.y = std::min(min.y, head.y);

				// print(grid, max_x, max_y, head, tail);
				// std::cout << "--------------" << std::endl;
			}
		}
	}

	auto visited = print(grid, min, max, head, tail);

	return visited;
}

uint64_t puzzle_09_2(std::ifstream &in_file)
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

uint64_t puzzle_09(std::ifstream &in_file)
{
	return puzzle_09_1(in_file);
}
