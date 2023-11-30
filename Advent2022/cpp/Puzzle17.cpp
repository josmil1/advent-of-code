#include "Puzzles.hpp"

namespace
{
using Grid = std::unordered_map<uint64_t, bool>;
inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<uint64_t>(x) << 32 | static_cast<uint64_t>(y);
}

struct Position
{
	uint32_t x{0};
	uint32_t y{0};
};

struct Piece
{
	std::vector<Position> pattern;
	Position              max;
};

void print(Grid &grid, uint32_t max_x, uint32_t max_y)
{
	// Print from top to bottom
	for (int j = max_y; j >= 0; j--)
	{
		for (int i = 0; i <= max_x; i++)
		{
			if (grid[key(i, j)])
			{
				std::cout << '#';
			}
			else
			{
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

std::vector<Piece> get_pieces()
{
	std::vector<Piece> pieces;
	pieces.resize(5);

	// ####
	pieces[0].pattern = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
	pieces[0].max     = {3, 0};

	// .#.
	// ###
	// .#.
	pieces[1].pattern = {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};
	pieces[1].max     = {2, 2};

	// ..#
	// ..#
	// ###
	pieces[2].pattern = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
	pieces[2].max     = {2, 2};

	// #
	// #
	// #
	// #
	pieces[3].pattern = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
	pieces[3].max     = {0, 3};

	// ##
	// ##
	pieces[4].pattern = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	pieces[4].max     = {1, 1};

	return pieces;
}

void descend(Grid &grid, Position *grid_max, Piece &piece, std::string &gas_pattern, uint32_t *gas_i)
{
	constexpr int32_t initial_x_offset{2};
	constexpr int32_t initial_y_offset{-3};

	Position final_offset{initial_x_offset, 0};

	uint32_t max_offset_x{grid_max->x - piece.max.x + 1};

	// In the grid, 0 is floor level, new piece starts above highest point
	bool horizontal_collision{false}, vertical_collision{false};
	for (int32_t offset_y = initial_y_offset; offset_y <= static_cast<int32_t>(grid_max->y); offset_y++)
	{
		if (vertical_collision)
		{
			break;
		}

		// Calculate horizontal shift
		auto    c            = gas_pattern[*gas_i % gas_pattern.size()];
		bool    left         = '<' == c;
		int32_t new_offset_x = left ? final_offset.x - 1 : final_offset.x + 1;
		int32_t new_offset_y = grid_max->y - offset_y + 1;
		(*gas_i)++;

		std::cout << "tentative offset x: " << new_offset_x << " y: " << new_offset_y << std::endl;

		for (auto pos : piece.pattern)
		{
			int32_t pos_x = pos.x + new_offset_x;
			int32_t pos_y = pos.y + final_offset.y;        // Not considering the down move yet

			if (new_offset_x < 0 || new_offset_x >= max_offset_x ||        // Outside walls
			    grid[key(pos_x, pos_y)])                                   // Collision with other piece
			{
				// Ignore move
				horizontal_collision = true;
				pos_x                = pos.x;
				std::cout << "horizontal collision at (" << pos_x << "," << pos_y << ") new_offset " << new_offset_x << " max_offset_x " << max_offset_x << std::endl;
			}
			else
			{
				final_offset.x = new_offset_x;
			}

			pos_y = pos.y + new_offset_y;        // Move down

			if (grid[key(pos_x, pos_y)])        // Collision with other piece
			{
				// Ignore move and stop moving down
				vertical_collision = true;
				std::cout << "vertical collision" << std::endl;
				break;
			}
			else
			{
				final_offset.y = new_offset_y;
			}
		}
	}

	// Record final position of piece in grid
	for (auto pos : piece.pattern)
	{
		int32_t pos_x           = pos.x + final_offset.x;
		int32_t pos_y           = pos.y + final_offset.y;
		grid[key(pos_x, pos_y)] = true;
	}

	grid_max->y = std::max(grid_max->y, final_offset.y) + piece.max.y;

	std::cout << "final_offset = (" << final_offset.x << "," << final_offset.y << ")" << std::endl;

	print(grid, grid_max->x, grid_max->y);

	std::cout << "---------" << std::endl;
}

uint64_t puzzle_17_1(std::ifstream &in_file)
{
	Grid grid;
	auto pieces = get_pieces();

	// Parse input
	std::string gas_pattern;
	while (std::getline(in_file, gas_pattern))
	{
		if (gas_pattern.size() > 0)
		{
			break;
		}
	}

	// Simulation
	constexpr uint32_t piece_limit{3};
	constexpr uint32_t grid_max_x{6};
	Position           grid_max{grid_max_x, 0};
	uint32_t           gas_i{0};
	for (uint32_t i = 0; i < piece_limit; i++)
	{
		auto &piece = pieces[i % pieces.size()];

		descend(grid, &grid_max, piece, gas_pattern, &gas_i);
	}

	return 0;
}

uint64_t puzzle_17_2(std::ifstream &in_file)
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

uint64_t puzzle_17(std::ifstream &in_file)
{
	return puzzle_17_1(in_file);
}
