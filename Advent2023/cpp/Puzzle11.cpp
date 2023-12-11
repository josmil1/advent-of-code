#include "Puzzles.hpp"

#include <set>

namespace
{
uint32_t EXPANSION_FACTOR{2};

struct Pos
{
	uint32_t x;
	uint32_t y;
};

uint64_t puzzle_11_1(std::ifstream &in_file)
{
	std::string line;

	std::unordered_map<uint32_t, Pos> galaxies;
	std::set<uint32_t>                galaxy_rows, galaxy_columns;

	uint32_t line_count{0}, line_width{0};

	uint32_t galaxy_count{0};

	// Parse map
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			for (uint32_t i = 0; i < line.size(); i++)
			{
				if ('#' == line[i])
				{
					galaxies[galaxy_count] = {i, line_count};

					galaxy_count++;

					galaxy_rows.insert(line_count);
					galaxy_columns.insert(i);
				}
			}

			line_count++;
			if (0 == line_width)
			{
				line_width = line.size();
			}
		}
	}

	// Sum shortest distance between galaxy combinations
	uint64_t sum{0};
	for (uint32_t i = 0; i < galaxy_count; i++)
	{
		for (uint32_t j = i + 1; j < galaxy_count; j++)
		{
			uint32_t distance_x = std::abs(static_cast<int32_t>(galaxies[i].x) - static_cast<int32_t>(galaxies[j].x));

			uint32_t distance_y = std::abs(static_cast<int32_t>(galaxies[i].y) - static_cast<int32_t>(galaxies[j].y));

			uint32_t empty_columns = distance_x - std::abs(std::distance(galaxy_columns.begin(), std::find(galaxy_columns.begin(), galaxy_columns.end(), galaxies[i].x)) -
			                                               std::distance(galaxy_columns.begin(), std::find(galaxy_columns.begin(), galaxy_columns.end(), galaxies[j].x)));

			uint32_t empty_rows = distance_y - std::abs(std::distance(galaxy_rows.begin(), std::find(galaxy_rows.begin(), galaxy_rows.end(), galaxies[i].y)) -
			                                            std::distance(galaxy_rows.begin(), std::find(galaxy_rows.begin(), galaxy_rows.end(), galaxies[j].y)));

			uint64_t distance = distance_x + distance_y + (EXPANSION_FACTOR - 1) * (empty_columns + empty_rows);

			sum += distance;
		}
	}

	return sum;
}

uint64_t puzzle_11_2(std::ifstream &in_file)
{
	EXPANSION_FACTOR = 1000000;

	return puzzle_11_1(in_file);
}
}        // namespace

uint64_t puzzle_11(std::ifstream &in_file)
{
	return puzzle_11_2(in_file);
}
