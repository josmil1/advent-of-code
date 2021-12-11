#include "Puzzles.hpp"

#define toDigit(c) (c - '0')

namespace
{
    uint64_t solve(std::ifstream &in_file, bool part1)
    {
        // Parse input
        std::string line;
        std::vector<std::string> grid;
        while (std::getline(in_file, line))
        {
            if (line.size() > 0)
            {
                grid.push_back(line);
            }
        }
        auto col_count = grid[0].length();
        auto row_count = grid.size();

        // Find low points
        std::vector<std::pair<uint32_t, uint32_t>> low_points;
        for (uint32_t i = 0; i < row_count; i++)
        {
            for (uint32_t j = 0; j < col_count; j++)
            {
                uint32_t current = toDigit(grid[i][j]);
                int32_t left = -1, right = -1, top = -1, bottom = -1;
                if (i > 0)
                {
                    top = toDigit(grid[i - 1][j]);
                }
                if (i < row_count - 1)
                {
                    bottom = toDigit(grid[i + 1][j]);
                }
                if (j > 0)
                {
                    left = toDigit(grid[i][j - 1]);
                }
                if (j < col_count)
                {
                    right = toDigit(grid[i][j + 1]);
                }

                if ((top < 0 || top > current) &&
                    (bottom < 0 || bottom > current) &&
                    (left < 0 || left > current) &&
                    (right < 0 || right > current))
                {
                    uint32_t risk = current + 1;
                    low_points.push_back({current, risk});
                }
            }
        }

        uint64_t total_risk_p1 = std::accumulate(low_points.begin(), low_points.end(), 0, [&](auto &left, auto &right)
                                                 { return left + right.second; });

        return total_risk_p1;
    }
}

uint64_t puzzle_09(std::ifstream &in_file)
{
    return solve(in_file, true);
}
