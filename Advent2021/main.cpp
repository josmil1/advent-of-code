//
//  main.cpp
//  Advent2021
//
//  Created by Jose Emilio Muñoz Lopez on 02/12/2021.
//

#include "Puzzles.hpp"

#define DAY 09

#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)
#define CONC_INDIR(A, B) A##B
#define CONC(A, B) CONC_INDIR(A, B)

#define FUNC_NAME CONC(puzzle_, DAY)

int main(int argc, const char *argv[])
{
    std::string input_file = "input_" + std::string(STR(DAY)) + ".txt";
    std::ifstream in_file(input_file);
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return 1;
    }

    auto answer = FUNC_NAME(in_file);

    in_file.close();

    std::cout << "Answer: " << answer << std::endl;

    return 0;
}
