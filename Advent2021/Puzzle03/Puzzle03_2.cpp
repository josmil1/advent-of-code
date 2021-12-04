#include "Puzzles.hpp"

#include <bitset>

//#define BITSET_SIZE 5
#define BITSET_SIZE 12

namespace
{
    uint64_t find_value(std::vector<std::bitset<BITSET_SIZE>> *input_bitsets, uint64_t input_count, bool want_majority)
    {
        uint32_t current_bit = BITSET_SIZE - 1;
        uint64_t current_count = input_count;
        while (current_count > 1)
        {
            uint64_t current_set_count = 0;
            //std::cout << current_count << std::endl;
            for (auto &bits : *input_bitsets)
            {
                current_set_count += static_cast<uint32_t>(bits[current_bit]);
            }

            auto it = input_bitsets->begin();
            // Find out if majority/minority of bits in this position are set or not
            bool want_set = (want_majority && (current_set_count >= current_count - current_set_count)) ||
                            (!want_majority && (current_set_count < current_count - current_set_count));
            //std::cout << current_set_count << " >= " << current_count - current_set_count << std::endl;
            // Keep only bitsets with a bit set in this position if majority/minority were set, or unset otherwise
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
}

void puzzle_03_2()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle03/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    // Parse input
    std::string line;
    uint64_t input_count = 0;
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
    auto co2 = find_value(&co2_bitsets, input_count, false);

    std::cout << oxygen * co2 << std::endl;

    in_file.close();
}
