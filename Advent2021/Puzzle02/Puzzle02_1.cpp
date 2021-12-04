#include "Puzzles.hpp"

void puzzle_02_1()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle01/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    // Parse input
    std::string line;
    uint32_t increase_count = 0;
    int32_t prev_number = -1;
    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            uint32_t input_number = std::stoi(line);
            int32_t difference = input_number - prev_number;
            if (prev_number > 0 && difference > 0)
            {
                //std::cout << input_number << " - " << prev_number << " = " << difference << std::endl;
                increase_count++;
            }
            prev_number = input_number;
        }
    }

    std::cout << increase_count << std::endl;

    in_file.close();
}
