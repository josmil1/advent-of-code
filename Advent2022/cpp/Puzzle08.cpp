#include "Puzzles.hpp"

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

struct HorizontalHelpers
{
	uint32_t max_height{0};
	DigitMap max_pos_for_height;
};

struct VerticalHelpers
{
	std::vector<uint32_t> max_height;
	std::vector<DigitMap> max_pos_for_height;
};

uint32_t update_visibility_count(uint32_t height, uint32_t pos, DigitMap *max_pos_for_height, uint32_t *max_height)
{
	// Visibility in this direction is defined as the distance to the closest tree of equal or greater height
	uint32_t count = std::abs(static_cast<int32_t>(pos) - static_cast<int32_t>((*max_pos_for_height)[height]));

	// Keep track of how far is the first tree that blocks each possible height
	for (uint32_t i = 0; i <= height; i++)
	{
		(*max_pos_for_height)[i] = pos;
	}

	*max_height = std::max(*max_height, height);

	return count;
}

constexpr uint32_t MAX_HEIGHT = 9;

TreeGrid build_grid(std::ifstream &in_file)
{
	std::string       line;
	TreeGrid          grid;
	uint32_t          grid_size{0};
	HorizontalHelpers left{};
	VerticalHelpers   top{};

	// Determine visibility from left/top while parsing the input
	uint32_t row{0};
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			// Initialize helper structures
			for (int i = 0; i <= MAX_HEIGHT; i++)
			{
				left.max_pos_for_height[i] = 0;
			}
			left.max_height = 0;

			if (0 == grid_size)
			{
				grid_size = static_cast<uint32_t>(line.size());
				grid.reserve(grid_size);
				top.max_height.resize(grid_size);
				top.max_pos_for_height.reserve(grid_size);

				for (int32_t col = 0; col < grid_size; col++)
				{
					DigitMap new_digit_map;
					for (int i = 0; i <= MAX_HEIGHT; i++)
					{
						new_digit_map[i] = 0;
					}
					top.max_pos_for_height.push_back(new_digit_map);
				}
			}

			// Process lines, filling rows of grid
			std::vector<Tree> new_row;
			new_row.reserve(grid_size);
			for (int32_t col = 0; col < grid_size; col++)
			{
				Tree new_tree{};
				new_tree.height = line[col] - '0';

				// Part 1
				new_tree.visible = 0 == col || 0 == row ||                                                            // left/top edge
				                   grid_size - 1 == col || grid_size - 1 == row ||                                    // right/bottom edge
				                   new_tree.height > left.max_height || new_tree.height > top.max_height[col];        // inner

				// Part 2
				new_tree.visible_left = update_visibility_count(new_tree.height, col, &left.max_pos_for_height, &left.max_height);
				new_tree.visible_top  = update_visibility_count(new_tree.height, row, &top.max_pos_for_height[col], &top.max_height[col]);

				new_row.push_back(new_tree);
			}

			grid.push_back(new_row);
			row++;
		}
	}

	return grid;
}

void process_grid(TreeGrid &grid, uint32_t *total_visible, uint32_t *max_score)
{
	uint32_t          grid_size = static_cast<uint32_t>(grid.size());
	HorizontalHelpers right{};
	VerticalHelpers   bottom{};

	// Initialize helper structures
	for (int32_t col = 0; col < grid_size; col++)
	{
		DigitMap new_digit_map;
		for (int i = 0; i <= MAX_HEIGHT; i++)
		{
			new_digit_map[i] = grid_size - 1;
		}
		bottom.max_pos_for_height.push_back(new_digit_map);
	}
	bottom.max_height.resize(grid_size);

	// Determine visibility from right/bottom, and count totals
	for (int32_t row = grid_size - 1; row >= 0; row--)
	{
		for (int i = 0; i <= MAX_HEIGHT; i++)
		{
			right.max_pos_for_height[i] = grid_size - 1;
		}
		right.max_height = 0;

		for (int32_t col = grid_size - 1; col >= 0; col--)
		{
			auto &tree = grid[row][col];

			// Part 1
			tree.visible |= tree.height > right.max_height || tree.height > bottom.max_height[col];
			if (tree.visible)
			{
				(*total_visible)++;
			}

			// Part 2
			tree.visible_right  = update_visibility_count(tree.height, col, &right.max_pos_for_height, &right.max_height);
			tree.visible_bottom = update_visibility_count(tree.height, row, &bottom.max_pos_for_height[col], &bottom.max_height[col]);
			uint32_t score      = tree.visible_left * tree.visible_right * tree.visible_bottom * tree.visible_top;
			*max_score          = std::max(*max_score, score);
		}
	}
}

uint64_t puzzle_08_1(std::ifstream &in_file)
{
	uint32_t total_visible{0};
	uint32_t max_score{0};
	auto     grid = build_grid(in_file);
	process_grid(grid, &total_visible, &max_score);

	return total_visible;
}

uint64_t puzzle_08_2(std::ifstream &in_file)
{
	uint32_t total_visible{0};
	uint32_t max_score{0};
	auto     grid = build_grid(in_file);
	process_grid(grid, &total_visible, &max_score);

	return max_score;
}
}        // namespace

uint64_t puzzle_08(std::ifstream &in_file)
{
	return puzzle_08_2(in_file);
}
