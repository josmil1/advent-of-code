#include "Puzzles.hpp"

namespace
{
uint64_t puzzle_01_1(std::ifstream &in_file)
{
	std::string        line;
	uint64_t           sum         = 0;
	constexpr uint32_t digit_count = 2;

	while (!in_file.eof())
	{
		std::getline(in_file, line);
		std::string digits;

		for (auto c : line)
		{
			int digit = c - '0';
			if (digit < 10)
			{
				if (digits.size() == digit_count)
				{
					digits.pop_back();
				}
				digits += c;
			}
		}

		// 1 digit case
		if (digits.size() < digit_count)
		{
			digits += digits[0];
		}

		sum += std::stol(digits);
	}

	return sum;
}

uint64_t puzzle_01_2(std::ifstream &in_file)
{
	std::string              line;
	uint64_t                 sum{0};
	std::vector<std::string> string_digits{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

	while (!in_file.eof())
	{
		std::getline(in_file, line);
		std::string digits{"00"};

		// Find string digits
		int last_pos{-1}, first_pos{std::numeric_limits<int>::max()};

		for (int d = 0; d < string_digits.size(); d++)
		{
			int new_digit = (d + 1) + '0';

			// First
			int pos = line.find(string_digits[d]);
			if (pos != std::string::npos)
			{
				if (pos < first_pos)
				{
					first_pos    = pos;
					digits.at(0) = new_digit;
				}
			}

			// Last
			pos = line.rfind(string_digits[d]);
			if (pos != std::string::npos)
			{
				if (pos > last_pos)
				{
					last_pos     = pos;
					digits.at(1) = new_digit;
				}
			}
		}

		// Find numeric digits and replace if they occur before/after string ones
		for (int d = 0; d < line.size(); d++)
		{
			int new_digit = line[d] - '0';

			if (new_digit < 10)
			{
				// First
				if (d < first_pos)
				{
					digits.at(0) = line[d];
					first_pos    = d;
				}

				// Last
				if (d > last_pos)
				{
					digits.at(1) = line[d];
					last_pos     = d;
				}
			}
		}

		sum += std::stol(digits);
	}

	return sum;
}
}        // namespace

uint64_t puzzle_01(std::ifstream &in_file)
{
	return puzzle_01_2(in_file);
}
