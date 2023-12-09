#include "Puzzles.hpp"

#include <set>

namespace
{
using Sequence = std::vector<int64_t>;

int64_t get_last_value(Sequence &seq, int64_t last_value)
{
	std::set<int64_t> unique_values(seq.begin(), seq.end());

	if (unique_values.size() > 1)
	{
		Sequence reduction;
		for (size_t i = 0; i < seq.size() - 1; i++)
		{
			auto diff = seq[i + 1] - seq[i];
			reduction.push_back(diff);

			// std::cout << diff << " ";
		}
		// std::cout << std::endl
		//           << "get_last(reduction, " << seq.back() << std::endl;

		return get_last_value(reduction, seq.back() + last_value);
	}
	else
	{
		// std::cout << "return " << last_value << " + " << seq.back() << std::endl;
		return seq.back() + last_value;
	}
}

uint64_t puzzle_09_1(std::ifstream &in_file)
{
	int64_t sum{0};

	std::string line;

	std::vector<Sequence> histories;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			int64_t           value;
			Sequence          history;

			while (ss >> value)
			{
				history.push_back(value);
			}

			histories.push_back(history);
		}
	}

	for (auto &h : histories)
	{
		sum += get_last_value(h, 0);
	}

	return sum;
}

uint64_t puzzle_09_2(std::ifstream &in_file)
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

uint64_t puzzle_09(std::ifstream &in_file)
{
	return puzzle_09_1(in_file);
}
