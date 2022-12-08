#include "Puzzles.hpp"

#include <set>

namespace
{
struct Tree
{
	uint32_t height{0};
	bool     visible{false};         // Part 1
	uint32_t visible_left{0};        // Part 2
	uint32_t visible_right{0};
	uint32_t visible_top{0};
	uint32_t visible_bottom{0};
};

using TreeGrid = std::vector<std::vector<Tree>>;

void print_grid(TreeGrid &grid)
{
	auto grid_size = grid.size();

	for (int row = 0; row < grid_size; row++)
	{
		for (int col = 0; col < grid_size; col++)
		{
			std::cout << grid[row][col].height;
			if (grid[row][col].visible)
			{
				std::cout << "o ";
			}
			else
			{
				std::cout << "x ";
			}
			std::cout << grid[row][col].visible_left << " ";
			std::cout << grid[row][col].visible_top << " ";
			std::cout << grid[row][col].visible_right << " ";
			std::cout << grid[row][col].visible_bottom << " ";
		}
		std::cout << std::endl;
	}
}

TreeGrid build_grid(std::ifstream &in_file)
{
	std::string line;
	TreeGrid    columns;
	uint32_t    grid_size{0};

	// Determine visibility from left/top while parsing the input
	std::vector<uint32_t> max_height_from_top;        // Part 1
	std::vector<uint32_t> top_count;                  // Part 2
	std::vector<uint32_t> max_height_from_top_pos;
	std::vector<uint32_t> prev_top_height;
	uint32_t              row_i{0};
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == grid_size)
			{
				grid_size = static_cast<uint32_t>(line.size());
				columns.reserve(grid_size);
				max_height_from_top.resize(grid_size);
				top_count.resize(grid_size);
				prev_top_height.resize(grid_size);
				max_height_from_top_pos.resize(grid_size);
			}

			std::vector<Tree> row;
			row.reserve(grid_size);

			// Part 1
			uint32_t max_height_from_left{0};

			// Part 2
			uint32_t left_count{0};
			uint32_t prev_left_height{0};

			uint32_t max_height_from_left_pos{0};

			for (int32_t i = 0; i < grid_size; i++)
			{
				uint32_t height = line[i] - '0';

				Tree new_tree{};
				new_tree.height = height;

				// Part 1
				bool visible = 0 == i || 0 == row_i ||                                                  // left/top edge
				               grid_size - 1 == i || grid_size - 1 == row_i ||                          // right/bottom edge
				               height > max_height_from_left || height > max_height_from_top[i];        // inner
				new_tree.visible = visible;

				// Part 2
				if (0 == i)
				{
					left_count = 0;
				}
				else if (height > max_height_from_left)
				{
					// Taller than all until now
					left_count = i;
				}
				else if (height > prev_left_height)
				{
					left_count = i - max_height_from_left_pos;
				}
				else
				{
					left_count = 1;
				}
				/*
				else
				{
				    if (prev_left_height >= height)
				    {
				        // Left tree blocks previous ones
				        left_count = 1;
				    }
				    else
				    {
				        // Visible up to block
				        left_count++;
				    }
				}
				*/
				new_tree.visible_left = left_count;
				prev_left_height      = height;

				if (0 == row_i)
				{
					top_count[i] = 0;
				}
				else if (height > max_height_from_top[i])
				{
					// Taller than all until now
					top_count[i] = row_i;
				}
				else if (height > prev_top_height[i])
				{
					top_count[i] = row_i - max_height_from_top_pos[i];
				}
				else
				{
					top_count[i] = 1;
				}
				/*
				else
				{
				    if (prev_top_height[i] >= height)
				    {
				        // Top tree blocks previous ones
				        top_count[i] = 1;
				    }
				    else
				    {
				        // Visible up to block
				        top_count[i]++;
				    }
				}
				*/
				new_tree.visible_top = top_count[i];
				prev_top_height[i]   = height;

				if (height >= max_height_from_left)
				{
					max_height_from_left_pos = i;
				}
				max_height_from_left = std::max(max_height_from_left, height);
				if (height >= max_height_from_top[i])
				{
					max_height_from_top_pos[i] = row_i;
				}
				max_height_from_top[i] = std::max(max_height_from_top[i], height);

				row.push_back(new_tree);
			}

			columns.push_back(row);
			row_i++;
		}
	}

	return columns;
}

uint64_t puzzle_08_1(std::ifstream &in_file)
{
	// Determine visibility from right/bottom, and count total
	auto                  grid          = build_grid(in_file);
	uint32_t              grid_size     = static_cast<uint32_t>(grid.size());
	uint64_t              total_visible = 0;
	std::vector<uint32_t> max_height_from_bottom;
	max_height_from_bottom.resize(grid_size);
	std::vector<uint32_t> max_height_from_bottom_pos;
	max_height_from_bottom_pos.resize(grid_size, grid_size - 1);
	std::vector<uint32_t> bottom_count;
	std::vector<uint32_t> prev_bottom_height;
	bottom_count.resize(grid_size);
	prev_bottom_height.resize(grid_size);
	uint64_t max_score{0};
	for (int32_t row = grid_size - 1; row >= 0; row--)
	{
		uint32_t right_count{0};
		uint32_t max_height_from_right_pos{grid_size - 1};
		uint32_t max_height_from_right{0};
		uint32_t prev_right_height{0};
		for (int32_t col = grid_size - 1; col >= 0; col--)
		{
			auto height = grid[row][col].height;

			if (grid_size - 1 == col)
			{
				max_height_from_right = height;
			}
			if (grid_size - 1 == row)
			{
				max_height_from_bottom[col] = height;
			}

			grid[row][col].visible |= col == 0 || row == 0 ||        // Left/top edge
			                          height > max_height_from_right || height > max_height_from_bottom[col];

			// Part 2
			if (grid_size - 1 == col)
			{
				right_count = 0;
			}
			else if (height > max_height_from_right)
			{
				// Taller than all until now
				right_count = grid_size - 1 - col;
			}
			else if (height > prev_right_height)
			{
				right_count = max_height_from_right_pos - col;
			}
			else
			{
				right_count = 1;
			}
			/*
			else
			{
			    if (prev_right_height >= height)
			    {
			        // Right tree blocks previous ones
			        right_count = 1;
			    }
			    else
			    {
			        // Visible up to block
			        right_count++;
			    }
			}
			*/
			grid[row][col].visible_right = right_count;
			prev_right_height            = height;

			if (grid_size - 1 == row)
			{
				bottom_count[col] = 0;
			}
			else if (height > max_height_from_bottom[col])
			{
				// Taller than all until now
				bottom_count[col] = grid_size - 1 - row;
			}
			else if (height > prev_bottom_height[col])
			{
				bottom_count[col] = max_height_from_bottom_pos[col] - row;
			}
			else
			{
				bottom_count[col] = 1;
			}
			/*
			else
			{
			    if (prev_bottom_height[col] >= height)
			    {
			        // Bottom tree blocks previous ones
			        bottom_count[col] = 1;
			    }
			    else
			    {
			        // Visible up to block
			        bottom_count[col]++;
			    }
			}
			*/
			grid[row][col].visible_bottom = bottom_count[col];
			prev_bottom_height[col]       = height;

			if (height >= max_height_from_right)
			{
				max_height_from_right_pos = col;
			}
			max_height_from_right = std::max(max_height_from_right, height);
			if (height >= max_height_from_bottom[col])
			{
				max_height_from_bottom_pos[col] = row;
			}
			max_height_from_bottom[col] = std::max(max_height_from_bottom[col], height);

			if (grid[row][col].visible)
			{
				total_visible++;
			}

			auto    &tree  = grid[row][col];
			uint64_t score = tree.visible_left * tree.visible_right * tree.visible_bottom * tree.visible_top;
			max_score      = std::max(max_score, score);
		}
	}

	print_grid(grid);

	return max_score;

	return total_visible;
}

uint64_t puzzle_08_2(std::ifstream &in_file)
{
	// Determine score from right/bottom, and find max
	auto     grid = build_grid(in_file);
	uint64_t max_score{0};

	print_grid(grid);

	return max_score;
}
}        // namespace

uint64_t puzzle_08(std::ifstream &in_file)
{
	return puzzle_08_1(in_file);
}
