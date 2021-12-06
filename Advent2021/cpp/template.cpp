#include "Puzzles.hpp"

namespace
{
    uint64_t solve(std::ifstream &in_file, bool part1)
    {
        // Parse input
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
}

uint64_t puzzle_00(std::ifstream &in_file)
{
    return solve(in_file, true);
}
