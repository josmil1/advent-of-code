#include "Puzzles.hpp"

#include <map>

#define DAY_COUNT 256
#define MAX_AGE 9
#define MAX_ADULT_AGE 7

using AgeMap = std::map<uint32_t, uint64_t>;

void puzzle_06_1()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle06/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    AgeMap age_count;
    for (uint32_t i = 0; i < MAX_AGE; i++)
    {
        age_count[i] = 0;
    }

    // Parse input
    std::string line;

    // First line is list of initial ages
    std::getline(in_file, line);
    std::stringstream ss(line);
    std::vector<uint32_t> marked_numbers;
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        age_count[std::stoi(substr)]++;
    }

    for (uint32_t i = 0; i < MAX_AGE; i++)
    {
        //std::cout << "Initial State: " << i << " -> " << age_count[i] << std::endl;
    }

    for (uint32_t i = 0; i < DAY_COUNT; i++)
    {
        // Save modifications in a new map, so that they are
        // all simultaneous
        AgeMap new_age_count;
        for (uint32_t i = 0; i < MAX_AGE; i++)
        {
            new_age_count[i] = 0;
        }

        for (uint32_t j = 0; j < MAX_AGE; j++)
        {
            //std::cout << "After day " << i + 1 << ": " << j << " -> " << age_count[j] << std::endl;

            switch (j)
            {
            case 0:
                // Spawn new fish and reset timer to max adult age
                new_age_count[MAX_AGE - 1] += age_count[j];
                new_age_count[MAX_ADULT_AGE - 1] += age_count[j];
                break;

            default:
                new_age_count[j - 1] += age_count[j];
                break;
            }
        }

        // Update original map
        for (uint32_t i = 0; i < MAX_AGE; i++)
        {
            age_count[i] = new_age_count[i];
        }
    }

    // Count all fish
    uint64_t total_fish = 0;
    for (uint32_t i = 0; i < MAX_AGE; i++)
    {
        total_fish += age_count[i];
    }

    std::cout << total_fish << std::endl;

    in_file.close();
}
