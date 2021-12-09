#include "Puzzles.hpp"

#define UNIQUE_PATTERNS 10
#define OUTPUT_DIGITS 4

namespace
{
    uint64_t solve(std::ifstream &in_file, bool part1)
    {
        // 0 : 6
        // 1 : 2
        // 2 : 5
        // 3 : 5
        // 4 : 4
        // 5 : 5
        // 6 : 6
        // 7 : 3
        // 8 : 7
        // 9 : 6
        std::string line;
        uint32_t p1_answer = 0;
        while (std::getline(in_file, line))
        {
            if (line.size() > 0)
            {
                // Parse input
                std::vector<std::string> patterns;
                std::vector<std::string> output;
                std::stringstream ss(line);
                std::string str;
                for (uint32_t i = 0; i < UNIQUE_PATTERNS; i++)
                {
                    ss >> str;
                    patterns.push_back(str);
                    //std::cout << str << std::endl;
                }
                ss >> str; // Discard '|' separator
                // For part 1, count unique segment counts
                for (uint32_t i = 0; i < OUTPUT_DIGITS; i++)
                {
                    ss >> str;
                    output.push_back(str);
                    //std::cout << str << std::endl;
                    if ((str.length() > 1 && str.length() < 5) || str.length() == 7)
                    {
                        p1_answer++;
                    }
                }
            }
        }

        return p1_answer;
    }
}

uint64_t puzzle_08(std::ifstream &in_file)
{
    return solve(in_file, true);
}
