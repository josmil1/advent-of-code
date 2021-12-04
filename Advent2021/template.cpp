#include "Puzzles.hpp"

//const std::string addr_delim = "[";
//const std::string addr_delim_end = "]";
//const std::string value_delim = " = ";
//auto addr_start = line.find(addr_delim);
//auto value_start = line.find(value_delim);
//if (addr_start != std::string::npos)
//{
//    // Instruction is a mem write
//    auto addr_end = line.find(addr_delim_end);
//    std::string addr_str = line.substr(addr_start + addr_delim.size(), addr_end - addr_start - addr_delim.size());
//    uint64_t addr = std::stol(addr_str);

//    std::string value_str = line.substr(value_start + value_delim.size());
//    uint64_t value = std::stol(value_str);
//}

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

    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            std::cout << line << std::endl;
        }
    }

    in_file.close();
}
