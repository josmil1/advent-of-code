#include "Puzzles.hpp"

#include <bitset>

namespace
{
#define MASK_SIZE 36

    class FerrySystem
    {
        public:
            void update_masks(std::string mask)
            {
                set_mask.reset();
                reset_mask.set();
                //std::cout << mask << std::endl;

                auto set_pos = mask.find(set_bit);
                while(set_pos != std::string::npos)
                {
                    set_mask[MASK_SIZE - 1 - set_pos] = 1;
                    set_pos = mask.find(set_bit, set_pos + 1);
                    //std::cout << "set mask " << set_mask << std::endl;
                }

                auto reset_pos = mask.find(reset_bit);
                while(reset_pos != std::string::npos)
                {
                    reset_mask[MASK_SIZE - 1 - reset_pos] = 0;
                    reset_pos = mask.find(reset_bit, reset_pos + 1);
                    //std::cout << "reset mask " << reset_mask << std::endl;
                }
            }

            void write(uint64_t addr, uint64_t value)
            {
                //std::cout << "mem[" << addr << "] = " << value << std::endl;
                auto binary_value = std::bitset<MASK_SIZE>(value);
                auto binary_str = binary_value.to_string(); //to binary
                //std::cout << binary_str << "\n";

                binary_value &= reset_mask;
                binary_value |= set_mask;
                binary_str = binary_value.to_string(); //to binary
                //std::cout << binary_str << "\n";

                memory[addr] = binary_value;
            }

            uint64_t get_sum()
            {
                uint64_t sum = 0;
                for (auto& mem : memory)
                {
                    sum += mem.second.to_ulong();
                }

                return sum;
            }

        private:
            const std::string set_bit{"1"};

            const std::string reset_bit{"0"};

            std::unordered_map<uint64_t, std::bitset<MASK_SIZE>> memory;

            std::bitset<MASK_SIZE> set_mask;

            std::bitset<MASK_SIZE> reset_mask;
    };
}

void puzzle_01_1()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle01/input.txt");
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
