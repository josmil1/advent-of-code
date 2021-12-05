#include "Puzzles.hpp"

namespace
{
    using Grid = std::unordered_map<uint64_t, uint32_t>;
    inline uint64_t key(uint32_t x, uint32_t y) { return static_cast<uint64_t>(x) << 32 | y; }
}

void puzzle_05_2()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle05/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    Grid floor;
    uint64_t overlap_count = 0;

    std::string line;
    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            // Parse input
            std::stringstream ss(line);
            uint32_t x_1, y_1, x_2, y_2;
            std::string sep;
            char comma;
            ss >> x_1 >> comma >> y_1 >> sep >> x_2 >> comma >> y_2;
            //std::cout << "(" << x_1 << ", " << y_1 << ") -> (" << x_2 << ", " << y_2 << ")" << std::endl;

            // Increment counters in grid
            // Assume parallel, or 45 degree angles only
            while (x_1 != x_2 || y_1 != y_2)
            {
                //std::cout << x_1 << ", " << x_2 << std::endl;
                floor[key(x_1, y_1)]++;
                if (y_1 != y_2)
                    y_1 = (y_1 < y_2) ? y_1 + 1 : y_1 - 1;
                if (x_1 != x_2)
                    x_1 = (x_1 < x_2) ? x_1 + 1 : x_1 - 1;
            }
            floor[key(x_1, y_1)]++; // Include last point
        }
    }

    overlap_count = std::count_if(
        floor.begin(),
        floor.end(),
        [](std::pair<uint64_t, uint32_t> v)
        { return v.second > 1; });

    std::cout << overlap_count << std::endl;

    in_file.close();
}
