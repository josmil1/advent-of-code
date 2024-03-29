#include "Puzzles.hpp"

namespace
{
using Grid = std::unordered_map<uint64_t, std::string>;

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<uint64_t>(x) << 32 | static_cast<uint32_t>(y);
}

struct Pos
{
	int32_t x{0};
	int32_t y{0};
};

void print(Grid &grid, uint64_t height, uint64_t width)
{
	// Print from top to bottom
	for (int32_t j = 0; j < height; j++)
	{
		int skip = 0;
		for (int32_t i = 0; i < width; i++)
		{
			auto grid_key = key(i, j);
			if (grid.count(grid_key))
			{
				std::cout << grid[grid_key];

				skip = grid[grid_key].size() - 1;
			}
			else if (skip > 0)
			{
				skip--;
			}
			else
			{
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

Grid parse_grid(std::ifstream &in_file, uint64_t &height, uint64_t &width)
{
	Grid        grid;
	uint64_t    line_count{0}, line_width{0};
	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			bool        new_number{false};
			std::string number;
			int32_t     number_x{0};

			for (int x = 0; x < line.size(); x++)
			{
				if (std::isdigit(line.at(x)))
				{
					number += line[x];

					if (!new_number)
					{
						new_number = true;
						number_x   = x;
					}
				}

				if (!std::isdigit(line.at(x)) || x == line.size() - 1)        // Consider end of the line
				{
					// Check if there's a number to store
					if (new_number)
					{
						grid[key(number_x, line_count)] = number;

						new_number = false;
						number.clear();
					}

					// Store symbol
					if (line[x] != '.' && x != line.size() - 1)
					{
						grid[key(x, line_count)] = line[x];
					}
				}
			}

			line_count++;
			if (0 == line_width)
			{
				line_width = line.size();
			}
		}
	}

	height = line_count;
	width  = line_width;

	return grid;
}

uint64_t puzzle_03_1(std::ifstream &in_file)
{
	uint64_t height{0}, width{0};
	auto     grid = parse_grid(in_file, height, width);
	uint64_t sum{0};

	for (int32_t j = 0; j < height; j++)
	{
		for (int32_t i = 0; i < width; i++)
		{
			auto grid_key = key(i, j);
			if (grid.count(grid_key))
			{
				std::string element = grid[grid_key];
				if (std::isdigit(element[0]))
				{
					// Number, check if it has a symbol around it
					for (int32_t x = i - 1; x <= static_cast<int32_t>(i + element.size()); x++)
					{
						for (int32_t y = j - 1; y <= j + 1; y++)
						{
							if (!(x == i && y == j) && grid.count(key(x, y)))        // Avoid detecting itself
							{
								if (!std::isdigit(grid[key(x, y)][0]))        // Avoid detecting another number
								{
									sum += std::stol(element);
								}
							}
						}
					}
				}
			}
		}
	}

	return sum;
}

uint64_t puzzle_03_2(std::ifstream &in_file)
{
	uint64_t height{0}, width{0};
	auto     grid = parse_grid(in_file, height, width);
	uint64_t sum{0};

	for (int32_t j = 0; j < height; j++)
	{
		for (int32_t i = 0; i < width; i++)
		{
			auto grid_key = key(i, j);
			if (grid.count(grid_key))
			{
				if (grid[grid_key] == "*")
				{
					// Gear, check if it has a number around it
					int      number_count{0};
					uint64_t gear_ratio{1};

					for (int32_t x = 0; x < width; x++)
					{
						for (int32_t y = j - 1; y <= j + 1; y++)
						{
							if (grid.count(key(x, y)))
							{
								auto &element = grid[key(x, y)];
								if (std::isdigit(element[0]))
								{
									// Consider a long number that spans above/below the gear
									auto number_size = static_cast<int32_t>(element.size());
									if ((x >= (i - 1) && x <= (i + 1)) || (x <= (i - 1) && x + number_size >= i))
									{
										number_count++;
										gear_ratio *= std::stol(element);
									}
								}
							}
						}
					}

					// A gear is any * symbol that is adjacent to exactly two part numbers
					if (2 == number_count)
					{
						// Its gear ratio is the result of multiplying those two numbers together
						sum += gear_ratio;
						gear_ratio = 1;
					}
				}
			}
		}
	}

	// Return the sum of all of the gear ratios in the engine schematic
	return sum;
}
}        // namespace

uint64_t puzzle_03(std::ifstream &in_file)
{
	return puzzle_03_2(in_file);
}
