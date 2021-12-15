#include "Puzzles.hpp"

#include <bitset>

//#define BITSET_SIZE 5
#define BITSET_SIZE 12

namespace
{
uint64_t find_value(std::vector<std::bitset<BITSET_SIZE>> *input_bitsets,
                    uint64_t input_count, bool want_majority)
{
	uint32_t current_bit   = BITSET_SIZE - 1;
	uint64_t current_count = input_count;
	while (current_count > 1)
	{
		uint64_t current_set_count = 0;
		// std::cout << current_count << std::endl;
		for (auto &bits : *input_bitsets)
		{
			current_set_count += static_cast<uint32_t>(bits[current_bit]);
		}

		auto it = input_bitsets->begin();
		// Find out if majority/minority of bits in this position are set or not
		bool want_set = (want_majority && (current_set_count >=
		                                   current_count - current_set_count)) ||
		                (!want_majority &&
		                 (current_set_count < current_count - current_set_count));
		// std::cout << current_set_count << " >= " << current_count -
		// current_set_count << std::endl;
		// Keep only bitsets with a bit set in this position if majority/minority
		// were set, or unset otherwise
		while (it != input_bitsets->end())
		{
			if ((want_set && !it->test(current_bit)) ||
			    (!want_set && it->test(current_bit)))
			{
				it = input_bitsets->erase(it);
			}
			else
			{
				++it;
			}
		}

		current_count = input_bitsets->size();
		current_bit--;
	}

	auto answer = input_bitsets->at(0);
	std::cout << answer << ": " << answer.to_ulong() << std::endl;

	return answer.to_ulong();
}

uint64_t puzzle_03_1(std::ifstream &in_file)
{
	// Parse input
	std::string           line;
	std::vector<uint32_t> bit_set_count;
	bit_set_count.resize(BITSET_SIZE);
	uint64_t input_count = 0;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::bitset<BITSET_SIZE> binary_input{line};
			for (uint32_t i = 0; i < binary_input.size(); i++)
			{
				bit_set_count[i] += static_cast<uint32_t>(binary_input[i]);
			}
			input_count++;
		}
	}

	std::bitset<BITSET_SIZE> gamma_rate, epsilon_rate;
	for (uint32_t i = 0; i < bit_set_count.size(); i++)
	{
		if (bit_set_count[i] > input_count / 2)
		{
			// Majority of bits in this position are set
			gamma_rate.set(i);
		}
	}
	epsilon_rate = gamma_rate;
	epsilon_rate.flip();

	std::cout << gamma_rate.to_ulong() << " * " << epsilon_rate.to_ulong()
	          << std::endl;

	return gamma_rate.to_ulong() * epsilon_rate.to_ulong();
}

uint64_t puzzle_03_2(std::ifstream &in_file)
{
	// Parse input
	std::string                           line;
	uint64_t                              input_count = 0;
	std::vector<std::bitset<BITSET_SIZE>> oxygen_bitsets, co2_bitsets;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::bitset<BITSET_SIZE> binary_input{line};
			oxygen_bitsets.push_back(binary_input);
			co2_bitsets.push_back(binary_input);

			input_count++;
		}
	}

	auto oxygen = find_value(&oxygen_bitsets, input_count, true);
	auto co2    = find_value(&co2_bitsets, input_count, false);

	return oxygen * co2;
}
}        // namespace

uint64_t puzzle_03(std::ifstream &in_file)
{
	return puzzle_03_2(in_file);
}
