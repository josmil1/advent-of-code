#include "Puzzles.hpp"

#include <set>

namespace
{
uint64_t puzzle_01_1(std::ifstream &in_file)
{
	std::string             line;
	std::multiset<uint64_t> left;
	std::multiset<uint64_t> right;

	// Parsing
	while (!in_file.eof())
	{
		std::getline(in_file, line);

		std::stringstream ss(line);

		uint64_t left_id;
		uint64_t right_id;

		ss >> left_id >> right_id;

		left.emplace(left_id);
		right.emplace(right_id);
	}

	// Solving
	auto     it1 = left.begin();
	auto     it2 = right.begin();
	uint64_t sum{0};

	while (it1 != left.end() && it2 != right.end())
	{
		int64_t i1 = *it1;
		int64_t i2 = *it2;

		it1++;
		it2++;

		auto diff = std::abs(i2 - i1);
		sum += diff;
	}

	return sum;
}

uint64_t puzzle_01_2(std::ifstream &in_file)
{
	std::string                            line;
	std::multiset<uint64_t>                left;
	std::unordered_map<uint64_t, uint64_t> right_count;

	// Parsing
	while (!in_file.eof())
	{
		std::getline(in_file, line);

		std::stringstream ss(line);

		uint64_t left_id;
		uint64_t right_id;

		ss >> left_id >> right_id;

		left.emplace(left_id);
		right_count[right_id]++;
	}

	// Solving
	uint64_t sum{0};

	for (auto i1 : left)
	{
		if (right_count.count(i1))
		{
			sum += i1 * right_count[i1];
		}
	}

	return sum;
}
}        // namespace

uint64_t puzzle_01(std::ifstream &in_file)
{
	return puzzle_01_2(in_file);
}
