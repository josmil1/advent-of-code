#include "Puzzles.hpp"

void puzzle_02_2()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle02/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    std::string line, command;
    uint32_t number;
    int64_t position = 0, depth = 0, aim = 0;

    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            // Parse input
            std::stringstream line_stream{line};
            line_stream >> command >> number;

            // Process commands
            if ("forward" == command)
            {
                position += number;
                depth += aim * number;
            }
            else if ("up" == command)
            {
                aim -= number;
            }
            else if ("down" == command)
            {
                aim += number;
            }
            else
            {
                std::cerr << "unknown command " << command << std::endl;
            }
        }
    }

    std::cout << depth * position << std::endl;

    in_file.close();
}
