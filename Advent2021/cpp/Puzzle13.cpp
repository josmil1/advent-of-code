#include "Puzzles.hpp"

namespace
{
    using Grid = std::unordered_map<uint64_t, bool>;
    inline uint64_t key(uint32_t x, uint32_t y) { return static_cast<uint64_t>(x) << 32 | y; }

    uint32_t print_paper(Grid &paper, uint32_t max_x, uint32_t max_y)
    {
        uint32_t visible = 0;
        for (uint32_t j = 0; j <= max_y; j++)
        {
            for (uint32_t i = 0; i <= max_x; i++)
            {
                auto paper_key = key(i, j);
                if (paper.count(paper_key) && paper[paper_key])
                {
                    std::cout << "#";
                    visible++;
                }
                else
                {
                    std::cout << ".";
                }
                std::cout << "";
            }
            std::cout << std::endl;
        }

        return visible;
    }

    uint64_t solve(std::ifstream &in_file, bool part1)
    {
        // Parse input
        std::string line;
        Grid paper;
        uint32_t x, y, axis, max_x = 0, max_y = 0;
        while (std::getline(in_file, line))
        {
            if (line.length() > 0)
            {
                std::stringstream ss(line);
                std::string sep, word;
                char symbol, character;
                ss >> word;
                if (word == "fold")
                {
                    auto visible = print_paper(paper, max_x, max_y);
                    std::cout << "Visible: " << visible << std::endl;

                    // Perform fold
                    ss >> word; // along
                    ss >> character;
                    ss >> symbol; // equals
                    ss >> axis;
                    if (character == 'x')
                    {
                        std::cout << "Fold along " << character << " = " << axis << std::endl;

                        for (uint32_t j = 0; j <= max_y; j++)
                        {
                            for (uint32_t i = axis + 1; i <= max_x; i++)
                            {
                                auto paper_key = key(i, j);
                                auto new_x = axis - (i - axis);
                                if (paper.count(paper_key) && paper[paper_key])
                                {
                                    //std::cout << "(" << i << ", " << j << ") -> (" << new_x << ", " << j << ")" << std::endl;
                                    paper[key(new_x, j)] = true;
                                }
                            }
                        }
                        max_x = axis - 1;
                    }
                    else if (character == 'y')
                    {
                        std::cout << "Fold along " << character << " = " << axis << std::endl;

                        for (uint32_t j = axis + 1; j <= max_y; j++)
                        {
                            for (uint32_t i = 0; i <= max_x; i++)
                            {
                                auto paper_key = key(i, j);
                                auto new_y = axis - (j - axis);
                                if (paper.count(paper_key) && paper[paper_key])
                                {
                                    //std::cout << "(" << i << ", " << j << ") -> (" << i << ", " << new_y << ")" << std::endl;
                                    paper[key(i, new_y)] = true;
                                }
                            }
                        }
                        max_y = axis - 1;
                    }
                    else
                    {
                        std::cerr << "Error: unreognised coordinate " << character << std::endl;
                    }
                }
                else
                {
                    // Load dots
                    ss = std::stringstream(line);
                    ss >> x >> symbol >> y; // comma
                    //std::cout << "(" << x << ", " << y << ")" << std::endl;
                    auto paper_key = key(x, y);
                    paper[paper_key] = true;
                    max_x = std::max(max_x, x);
                    max_y = std::max(max_y, y);
                }
            }
        }

        auto visible = print_paper(paper, max_x, max_y);

        return visible;
    }
}

uint64_t puzzle_13(std::ifstream &in_file)
{
    return solve(in_file, true);
}
