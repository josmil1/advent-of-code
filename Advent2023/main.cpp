//
//  main.cpp
//  Advent2023
//
//  Created by Jose Emilio Muñoz Lopez on 01/12/2023.
//

#include "Puzzles.hpp"

#define DAY 19

#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)
#define CONC_INDIR(A, B) A##B
#define CONC(A, B) CONC_INDIR(A, B)

#define FUNC_NAME CONC(puzzle_, DAY)

int main()
{
	std::string   input_file = "input/input_" + std::string(STR(DAY)) + ".txt";
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
