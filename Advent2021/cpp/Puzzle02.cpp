#include "Puzzles.hpp"

uint64_t puzzle_02_1(std::ifstream &in_file)
{
    std::string line, command;
    uint32_t number;
    uint64_t position = 0, depth = 0;

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
            }
            else if ("up" == command)
            {
                depth -= number;
            }
            else if ("down" == command)
            {
                depth += number;
            }
            else
            {
                std::cerr << "unknown command " << command << std::endl;
            }
        }
    }

    return depth * position;
}

uint64_t puzzle_02_2(std::ifstream &in_file)
{
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

    return depth * position;
}

uint64_t puzzle_02(std::ifstream &in_file)
{
    return puzzle_02_2(in_file);
}
