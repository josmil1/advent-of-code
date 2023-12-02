#include "Puzzles.hpp"

namespace
{
using ColorMap = std::unordered_map<std::string, uint32_t>;

bool check_color_counts(ColorMap &color_map)
{
	constexpr uint32_t MAX_RED   = 12;
	constexpr uint32_t MAX_GREEN = 13;
	constexpr uint32_t MAX_BLUE  = 14;

	for (auto &it : color_map)
	{
		if ("red" == it.first)
		{
			if (it.second > MAX_RED)
			{
				return false;
			}
		}

		if ("green" == it.first)
		{
			if (it.second > MAX_GREEN)
			{
				return false;
			}
		}

		if ("blue" == it.first)
		{
			if (it.second > MAX_BLUE)
			{
				return false;
			}
		}
	}

	return true;
}

uint64_t puzzle_02_1(std::ifstream &in_file)
{
	std::string line;

	uint64_t sum_possible_ids{0};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);

			int         game_id;
			ColorMap    color_count;
			std::string bin_str;
			bool        possible{true};

			// Game <id>:
			ss >> bin_str >> game_id >> bin_str;

			// x blue, y red, ...; u green, v blue (multiple lists per game)
			uint32_t    count;
			std::string color;
			while (ss >> count)
			{
				// New color count list
				ss >> color;
				bool list_continues = (',' == color.back());

				// The final list will not have a delimiter
				if (list_continues || (';' == color.back()))
				{
					color.pop_back();
				}

				color_count[color] = count;

				if (!list_continues)
				{
					possible &= check_color_counts(color_count);

					color_count.clear();
				}
			}

			// Sum IDs of possible games
			if (possible)
			{
				sum_possible_ids += game_id;
			}
		}
	}

	return sum_possible_ids;
}

uint64_t puzzle_02_2(std::ifstream &in_file)
{
	std::string line;

	uint64_t sum_set_powers{0};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);

			int         game_id;
			ColorMap    color_max;
			std::string bin_str;

			// Game <id>:
			ss >> bin_str >> game_id >> bin_str;

			// x blue, y red, ...; u green, v blue (multiple lists per game)
			color_max["blue"]  = 0;
			color_max["red"]   = 0;
			color_max["green"] = 0;

			uint32_t    count;
			std::string color;
			while (ss >> count)
			{
				ss >> color;
				if (',' == color.back() || (';' == color.back()))
				{
					color.pop_back();
				}

				color_max[color] = std::max(color_max[color], count);
			}

			// Sum set power
			sum_set_powers += std::accumulate(color_max.begin(), color_max.end(), 1,
			                                  [](uint64_t power, const ColorMap::value_type &s) { return power * s.second; });
		}
	}

	return sum_set_powers;
}
}        // namespace

uint64_t puzzle_02(std::ifstream &in_file)
{
	return puzzle_02_2(in_file);
}
