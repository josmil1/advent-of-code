#include "Puzzles.hpp"

#include <bitset>
#include <unordered_set>

namespace
{
#define MASK_SIZE 36

class FerrySystem
{
public:
    void update_masks(std::string mask)
    {
        set_mask.reset();
        auto set_pos = mask.find(set_bit);
        while(set_pos != std::string::npos)
        {
            set_mask[MASK_SIZE - 1 - set_pos] = 1;
            set_pos = mask.find(set_bit, set_pos + 1);
        }

        floating_bits.clear();
        auto float_pos = mask.find(float_bit);
        while(float_pos != std::string::npos)
        {
            floating_bits.push_back(MASK_SIZE - 1 - float_pos);
            float_pos = mask.find(float_bit, float_pos + 1);
        }
        auto n = static_cast<uint32_t>(floating_bits.size());
        std::string combination(n, '0');
        floating_combinations.clear();
        generate_floating_combinations(n, combination, 0);
    }

    void write(uint64_t addr, uint64_t value)
    {
        std::vector<std::string> debug_addr;
        auto addr_value = std::bitset<MASK_SIZE>(addr);
        addr_value |= set_mask;

        // Floating bits
        for (auto comb : floating_combinations)
        {
            auto new_addr_value = addr_value;
            for (uint32_t i = 0; i < floating_bits.size(); i++)
            {
                auto bit_pos = floating_bits[i];
                std::bitset<MASK_SIZE> new_mask;
                new_mask[bit_pos] = 1;
                // Set bit to 0
                new_addr_value &= ~new_mask;
                // Set bit to corresponding value in combination
                std::bitset<MASK_SIZE> float_mask;
                float_mask[bit_pos] = static_cast<uint32_t>(comb[i] - '0');
                new_addr_value |= float_mask;
            }
            auto addr_str = new_addr_value.to_string(); // debug
            //std::cout << addr_str << std::endl;
            memory[new_addr_value.to_ulong()] = value;
            debug_addr.push_back(addr_str);
        }
    }

    uint64_t get_sum()
    {
        uint64_t sum = 0;
        for (auto& mem : memory)
        {
            sum += mem.second;
        }

        return sum;
    }

private:
    const std::string set_bit{"1"};

    const std::string float_bit{"X"};

    std::unordered_map<uint64_t, uint64_t> memory;

    std::bitset<MASK_SIZE> set_mask;

    std::vector<size_t> floating_bits;

    std::vector<std::string> floating_combinations;

    // Function to generate all binary strings
    void generate_floating_combinations(uint32_t count, std::string combination, uint32_t i)
    {
        if (i == count) {
            floating_combinations.push_back(combination);
            return;
        }

        // First assign "0" at ith position
        // and try for all other permutations
        // for remaining positions
        combination[i] = '0';
        generate_floating_combinations(count, combination, i + 1);

        // And then assign "1" at ith position
        // and try for all other permutations
        // for remaining positions
        combination[i] = '1';
        generate_floating_combinations(count, combination, i + 1);
    }
};
}

void puzzle_14_2()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle14/input.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    // Parse input
    std::string line;
    const std::string addr_delim = "[";
    const std::string addr_delim_end = "]";
    const std::string value_delim = " = ";
    FerrySystem system;
    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            auto addr_start = line.find(addr_delim);
            auto value_start = line.find(value_delim);
            if (addr_start != std::string::npos)
            {
                // Instruction is a mem write
                auto addr_end = line.find(addr_delim_end);
                std::string addr_str = line.substr(addr_start + addr_delim.size(), addr_end - addr_start - addr_delim.size());
                uint64_t addr = std::stol(addr_str);

                std::string value_str = line.substr(value_start + value_delim.size());
                uint64_t value = std::stol(value_str);

                system.write(addr, value);
            }
            else
            {
                // Instruction is a mask update
                auto mask_str = line.substr(value_start + value_delim.size());

                system.update_masks(mask_str);
            }
        }
    }

    std::cout << system.get_sum() << std::endl;

    in_file.close();
}
