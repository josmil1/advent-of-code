#include "Puzzles.hpp"

void puzzle_03_2()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle03/input.txt");
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
