#include "Puzzles.hpp"

#include <bitset>

#define BITSET_SIZE 12

void puzzle_03_1()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle03/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    // Parse input
    std::string line;
    std::vector<uint32_t> bit_set_count;
    bit_set_count.resize(BITSET_SIZE);
    uint64_t input_count = 0;
    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            std::bitset<BITSET_SIZE> binary_input{line};
            for (int i = 0; i < binary_input.size(); i++)
            {
                bit_set_count[i] += static_cast<uint32_t>(binary_input[i]);
            }
            input_count++;
        }
    }

    std::bitset<BITSET_SIZE> gamma_rate, epsilon_rate;
    for (int i = 0; i < bit_set_count.size(); i++)
    {
        if (bit_set_count[i] > input_count / 2)
        {
            // Majority of bits in this position are set
            gamma_rate.set(i);
        }
    }
    epsilon_rate = gamma_rate;
    epsilon_rate.flip();

    std::cout << gamma_rate.to_ulong() << " * " << epsilon_rate.to_ulong() << std::endl;
    std::cout << gamma_rate.to_ulong() * epsilon_rate.to_ulong() << std::endl;

    in_file.close();
}
