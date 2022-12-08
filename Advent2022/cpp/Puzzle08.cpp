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

using DigitMap = std::unordered_map<uint32_t, uint32_t>;

struct VerticalHelpers
{
	std::vector<uint32_t> max_height{0};
	std::vector<uint32_t> max_height_pos{0};
	std::vector<uint32_t> prev_height{0};
	std::vector<uint32_t> count{0};

	std::vector<DigitMap> max_pos_for_digit;
};

struct HorizontalHelpers
{
	uint32_t max_height{0};
	uint32_t max_height_pos{0};
	uint32_t prev_height{0};
	uint32_t count{0};

	DigitMap max_pos_for_digit;
};

void update_visibility_count(uint32_t *max_height, uint32_t *max_height_pos, uint32_t *prev_height,
                             uint32_t *count, uint32_t height, uint32_t pos, uint32_t max_pos, DigitMap *max_pos_for_digit)
{
	// std::cout << "mp " << (*max_pos_for_digit)[height] << " ";
	*count = pos - (*max_pos_for_digit)[height];

	for (uint32_t i = 0; i <= height; i++)
	{
		(*max_pos_for_digit)[i] = pos;
	}

	/*
	if (0 == pos)
	{
	    *count = 0;
	}
	else if (height > *max_height)
	{
	    // Taller than all until now
	    *count = pos;
	}
	else if (height > *prev_height)
	{
	    *count = pos - *max_height_pos;
	}
	else
	{
	    *count = 1;
	}
	*prev_height = height;

	if (height >= *max_height)
	{
	    *max_height_pos = pos;
	}
	*max_height = std::max(*max_height, height);
	*/
}

uint32_t update_visibility_count(uint32_t height, uint32_t pos, DigitMap *max_pos_for_digit)
{
	std::cout << "mp " << (*max_pos_for_digit)[height] << " ";
	uint32_t count = std::abs(static_cast<int32_t>(pos) - static_cast<int32_t>((*max_pos_for_digit)[height]));

	for (uint32_t i = 0; i <= height; i++)
	{
		(*max_pos_for_digit)[i] = pos;
	}

	return count;
}

constexpr uint32_t MAX_HEIGHT = 9;

TreeGrid build_grid(std::ifstream &in_file)
{
	std::string line;
	TreeGrid    grid;
	uint32_t    grid_size{0};

	// Determine visibility from left/top while parsing the input
	VerticalHelpers top{};
	uint32_t        row{0};
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Initialize helper structures
			if (0 == grid_size)
			{
				grid_size = static_cast<uint32_t>(line.size());
				grid.reserve(grid_size);
				top.max_height.resize(grid_size);
				top.count.resize(grid_size);
				top.prev_height.resize(grid_size);
				top.max_height_pos.resize(grid_size);
				top.max_pos_for_digit.reserve(grid_size);
			}
			std::vector<Tree> new_row;
			new_row.reserve(grid_size);
			HorizontalHelpers left{};
			for (int i = 0; i <= MAX_HEIGHT; i++)
			{
				left.max_pos_for_digit[i] = 0;
			}
			for (int32_t col = 0; col < grid_size; col++)
			{
				DigitMap new_digit_map;
				for (int i = 0; i <= MAX_HEIGHT; i++)
				{
					new_digit_map[i] = 0;
				}
				top.max_pos_for_digit.push_back(new_digit_map);
			}

			// Process lines, filling rows of grid
			for (int32_t col = 0; col < grid_size; col++)
			{
				Tree new_tree{};
				new_tree.height = line[col] - '0';

				// Part 1
				new_tree.visible = 0 == col || 0 == row ||                                                            // left/top edge
				                   grid_size - 1 == col || grid_size - 1 == row ||                                    // right/bottom edge
				                   new_tree.height > left.max_height || new_tree.height > top.max_height[col];        // inner

				// Part 2
				update_visibility_count(&left.max_height, &left.max_height_pos, &left.prev_height, &left.count,
				                        new_tree.height, col, grid_size - 1, &left.max_pos_for_digit);
				new_tree.visible_left = left.count;

				update_visibility_count(&top.max_height[col], &top.max_height_pos[col], &top.prev_height[col], &top.count[col],
				                        new_tree.height, row, grid_size - 1, &(top.max_pos_for_digit[col]));
				new_tree.visible_top = top.count[col];

				new_row.push_back(new_tree);
			}

			grid.push_back(new_row);
			row++;
			// std::cout << std::endl;
		}
	}

	return grid;
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
	uint64_t        max_score{0};
	VerticalHelpers bottom{};
	for (int32_t col = 0; col < grid_size; col++)
	{
		DigitMap new_digit_map;
		for (int i = 0; i <= MAX_HEIGHT; i++)
		{
			new_digit_map[i] = grid_size - 1;
		}
		bottom.max_pos_for_digit.push_back(new_digit_map);
	}
	for (int32_t row = grid_size - 1; row >= 0; row--)
	{
		uint32_t          right_count{0};
		uint32_t          max_height_from_right_pos{grid_size - 1};
		uint32_t          max_height_from_right{0};
		uint32_t          prev_right_height{0};
		HorizontalHelpers right{};
		for (int i = 0; i <= MAX_HEIGHT; i++)
		{
			right.max_pos_for_digit[i] = grid_size - 1;
		}
		for (int32_t col = grid_size - 1; col >= 0; col--)
		{
			auto height = grid[row][col].height;
			std::cout << height;

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
			grid[row][col].visible_right  = update_visibility_count(height, col, &right.max_pos_for_digit);
			grid[row][col].visible_bottom = update_visibility_count(height, row, &(bottom.max_pos_for_digit[col]));

			/*
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
			grid[row][col].visible_right = right_count;
			prev_right_height            = height;
			*/

			/*
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
			*/

			if (grid[row][col].visible)
			{
				total_visible++;
			}

			auto    &tree  = grid[row][col];
			uint64_t score = tree.visible_left * tree.visible_right * tree.visible_bottom * tree.visible_top;
			max_score      = std::max(max_score, score);
		}
		std::cout << std::endl;
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
