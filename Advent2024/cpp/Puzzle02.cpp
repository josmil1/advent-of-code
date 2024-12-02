#include "Puzzles.hpp"

namespace
{
bool is_safe(std::string line)
{
	std::stringstream ss(line);

	int64_t level;
	int64_t prev_level;
	bool    init{false};
	bool    increasing{false};

	ss >> prev_level;

	while (ss >> level)
	{
		if (!init)
		{
			increasing = level > prev_level;
			init       = true;
		}

		auto diff      = std::abs(level - prev_level);
		auto safe_diff = diff > 0 && diff < 4;
		if (!safe_diff)
		{
			return false;
		}

		if ((level > prev_level && !increasing) ||
		    (level < prev_level) && increasing)
		{
			return false;
		}

		prev_level = level;
	}

	return true;
}

uint64_t puzzle_02_1(std::ifstream &in_file)
{
	std::string line;

	uint64_t safe_count{0};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (is_safe(line))
			{
				safe_count++;
			}
		}
	}

	return safe_count;
}

uint64_t puzzle_02_2(std::ifstream &in_file)
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

uint64_t puzzle_02(std::ifstream &in_file)
{
	return puzzle_02_1(in_file);
}
