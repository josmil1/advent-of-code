#include "Puzzles.hpp"

#include <set>

namespace
{
using Sequence = std::vector<int64_t>;

int64_t get_next_value(Sequence &seq)
{
	std::set<int64_t> unique_values(seq.begin(), seq.end());

	if (unique_values.size() > 1)
	{
		Sequence reduction;
		for (size_t i = 0; i < seq.size() - 1; i++)
		{
			auto diff = seq[i + 1] - seq[i];
			reduction.push_back(diff);
		}

		return seq.back() + get_next_value(reduction);
	}
	else
	{
		return seq.back();
	}
}

std::vector<Sequence> get_histories(std::ifstream &in_file)
{
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

	return histories;
}

uint64_t puzzle_09_1(std::ifstream &in_file)
{
	int64_t sum{0};

	auto histories = get_histories(in_file);

	for (auto &h : histories)
	{
		sum += get_next_value(h);
	}

	return sum;
}

uint64_t puzzle_09_2(std::ifstream &in_file)
{
	int64_t sum{0};

	auto histories = get_histories(in_file);

	for (auto &h : histories)
	{
		std::reverse(h.begin(), h.end());

		sum += get_next_value(h);
	}

	return sum;
}
}        // namespace

uint64_t puzzle_09(std::ifstream &in_file)
{
	return puzzle_09_2(in_file);
}
