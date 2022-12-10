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

void update_position(Pos *pos, const Pos ref)
{
	bool too_far_x        = abs(ref.x - pos->x) > 1;
	bool too_far_y        = abs(ref.y - pos->y) > 1;
	bool diff_row_and_col = ref.x != pos->x && ref.y != pos->y;
	if (too_far_x || too_far_y)
	{
		if (too_far_x || diff_row_and_col)
		{
			pos->x = close_gap(pos->x, ref.x);
		}

		if (too_far_y || diff_row_and_col)
		{
			pos->y = close_gap(pos->y, ref.y);
		}
	}
}

uint64_t solve(std::ifstream &in_file, const uint32_t knot_count)
{
	std::string line;

	Grid grid;
	Pos  min, max;
	Pos  knots[knot_count];
	Pos *head = &knots[0];
	Pos *tail = &knots[knot_count - 1];

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
						head->y++;
						break;
					}
					case 'D':
					{
						// Move down
						head->y--;
						break;
					}
					case 'L':
					{
						// Move left
						head->x--;
						break;
					}
					case 'R':
					{
						// Move right
						head->x++;
						break;
					}
					default:
					{
						std::cout << "Error, unknown command " << command << std::endl;
						break;
					}
				}

				for (int i = 1; i < knot_count; i++)
				{
					update_position(&knots[i], knots[i - 1]);
				}

				auto grid_key  = key(tail->x, tail->y);
				grid[grid_key] = true;

				max.x = std::max(max.x, head->x);
				max.y = std::max(max.y, head->y);
				min.x = std::min(min.x, head->x);
				min.y = std::min(min.y, head->y);
			}
		}
	}

	auto visited = print(grid, min, max, *head, *tail);

	return visited;
}

uint64_t puzzle_09_1(std::ifstream &in_file)
{
	return solve(in_file, 2);
}

uint64_t puzzle_09_2(std::ifstream &in_file)
{
	return solve(in_file, 10);
}
}        // namespace

uint64_t puzzle_09(std::ifstream &in_file)
{
	return puzzle_09_1(in_file);
}
