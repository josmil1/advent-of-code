#include "Puzzles.hpp"

namespace
{
    uint64_t solve(std::ifstream &in_file, bool part1)
    {
        uint64_t total_fuel_p1 = 0, total_fuel_p2 = 0;

        // Parse input
        std::string line;

        // First line is list of positions
        std::getline(in_file, line);
        std::stringstream ss(line);
        std::vector<uint32_t> positions;
        double sum = 0;
        while (ss.good())
        {
            std::string substr;
            std::getline(ss, substr, ',');
            int32_t new_position = std::stoi(substr);
            positions.push_back(new_position);
            sum += new_position;
        }

        // Part 1
        // Find median
        uint64_t n = positions.size() / 2;
        std::nth_element(positions.begin(), positions.begin() + n, positions.end());
        auto median = positions[n];

        // Find fuel spent to reach that position
        total_fuel_p1 = std::accumulate(positions.begin(), positions.end(), 0.0, [&](auto left, auto right)
                                        { return left + std::abs(static_cast<int>(right - median)); });

        // Part 2
        uint64_t average_ceil = std::ceil(sum / positions.size());
        uint64_t average_floor = std::floor(sum / positions.size());
        // Sum of first n positive integers = n(n + 1)/2
        uint64_t total_fuel_p2_ceil = std::accumulate(positions.begin(), positions.end(), 0.0, [&](uint64_t left, uint64_t right)
                                                      { return left + (std::abs(static_cast<int>(right - average_ceil)) * (std::abs(static_cast<int>(right - average_ceil)) + 1) / 2); });
        uint64_t total_fuel_p2_floor = std::accumulate(positions.begin(), positions.end(), 0.0, [&](uint64_t left, uint64_t right)
                                                       { return left + (std::abs(static_cast<int>(right - average_floor)) * (std::abs(static_cast<int>(right - average_floor)) + 1) / 2); });
        total_fuel_p2 = std::min(total_fuel_p2_ceil, total_fuel_p2_floor);

        return part1 ? total_fuel_p1 : total_fuel_p2;
    }
}

uint64_t puzzle_07(std::ifstream &in_file)
{
    return solve(in_file, false);
}
