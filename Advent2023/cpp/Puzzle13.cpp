#include "Puzzles.hpp"

namespace
{
struct Pos
{
	uint32_t x{0};
	uint32_t y{0};
};

using Grid = std::unordered_map<uint64_t, Pos>;

struct Pattern
{
	Grid     grid;
	uint32_t width;
	uint32_t height;
};

inline uint64_t key(uint32_t x, uint32_t y)
{
	return static_cast<uint64_t>(x) << 32 | y;
}

inline uint64_t key(Pos &pos)
{
	return key(pos.x, pos.y);
}

void print(Pattern &pattern)
{
	for (uint32_t j = 0; j < pattern.height; j++)
	{
		for (uint32_t i = 0; i < pattern.width; i++)
		{
			Pos pos{i, j};
			if (pattern.grid.count(key(pos)))
			{
				std::cout << '#';
			}
			else
			{
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
}

uint64_t get_reflection_score(Pattern &pattern, uint32_t smudges = 0)
{
	// Find column of reflection
	uint32_t reflection_column{0};
	for (uint32_t i = 1; i < pattern.width; i++)
	{
		uint32_t d{0}, smudge_count{0};
		bool     reflection{true};
		while (d < pattern.width - i)
		{
			int32_t i_left  = static_cast<int32_t>(i) - 1 - d;
			int32_t i_right = i + d;
			for (uint32_t j = 0; j < pattern.height; j++)
			{
				if (i_left >= 0 && i_right < pattern.width)
				{
					bool both_empty  = !pattern.grid.count(key(i_left, j)) && !pattern.grid.count(key(i_right, j));
					bool both_mirror = pattern.grid.count(key(i_left, j)) && pattern.grid.count(key(i_right, j));
					reflection &= (both_empty || both_mirror);
					if (!reflection && smudge_count < smudges)
					{
						reflection = true;
						smudge_count++;
					}
				}
			}

			if (reflection)
			{
				d++;
			}
			else
			{
				break;
			}
		}

		if (reflection && smudge_count == smudges)
		{
			reflection_column = i;
			break;
		}
	}

	if (reflection_column > 0)
	{
		return reflection_column;
	}

	// Find row of reflection
	uint32_t reflection_row{0};
	for (uint32_t j = 1; j < pattern.height; j++)
	{
		uint32_t d{0}, smudge_count{0};
		bool     reflection{true};
		while (d < pattern.height - j)
		{
			int32_t j_above = static_cast<int32_t>(j) - 1 - d;
			int32_t j_below = j + d;
			for (uint32_t i = 0; i < pattern.width; i++)
			{
				if (j_above >= 0 && j_below < pattern.height)
				{
					bool both_empty  = !pattern.grid.count(key(i, j_above)) && !pattern.grid.count(key(i, j_below));
					bool both_mirror = pattern.grid.count(key(i, j_above)) && pattern.grid.count(key(i, j_below));
					reflection &= (both_empty || both_mirror);
					if (!reflection && smudge_count < smudges)
					{
						reflection = true;
						smudge_count++;
					}
				}
			}

			if (reflection)
			{
				d++;
			}
			else
			{
				break;
			}
		}

		if (reflection && smudge_count == smudges)
		{
			reflection_row = j;
			break;
		}
	}

	return reflection_row * 100;
}

std::vector<Pattern> parse_patterns(std::ifstream &in_file)
{
	std::vector<Pattern> patterns;

	std::string line;
	Grid        grid;
	uint32_t    width{0}, height{0};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == width)
			{
				width = line.size();
			}

			for (uint32_t x = 0; x < width; x++)
			{
				if ('#' == line[x])
				{
					Pos mirror_pos{x, height};
					grid[key(mirror_pos)] = mirror_pos;
				}
			}

			height++;
		}

		if (line.empty() || in_file.eof())
		{
			patterns.push_back({grid, width, height});

			grid.clear();
			width  = 0;
			height = 0;
		}
	}

	return patterns;
}

uint64_t puzzle_13_1(std::ifstream &in_file)
{
	uint64_t sum{0};

	auto patterns = parse_patterns(in_file);

	for (auto &pattern : patterns)
	{
		auto reflection_score = get_reflection_score(pattern);

		sum += reflection_score;
	}

	return sum;
}

uint64_t puzzle_13_2(std::ifstream &in_file)
{
	uint64_t sum{0};

	auto patterns = parse_patterns(in_file);

	for (auto &pattern : patterns)
	{
		auto reflection_score = get_reflection_score(pattern, 1);

		sum += reflection_score;
	}

	return sum;
}
}        // namespace

uint64_t puzzle_13(std::ifstream &in_file)
{
	return puzzle_13_2(in_file);
}
