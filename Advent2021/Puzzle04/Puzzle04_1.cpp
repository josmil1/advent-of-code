#include "Puzzles.hpp"

#define ROW_SIZE 5
#define COL_SIZE 5

namespace
{
    class BingoCard
    {
    public:
        /**
         * @brief For initialisation: add number to the
         * bingo card
         */
        void add_number(uint32_t number, uint32_t row, uint32_t col)
        {
            card[number] = {row, col};

            //std::cout << "Added at " << card[number].row << ", " << card[number].col << std::endl;

            unmarked_numbers_sum += number;
        }

        /**
         * @brief Return true if the new number results
         * in a win (completed row/col)
         */
        bool mark_number(uint32_t number)
        {
            if (card.count(number))
            {
                // Number in card
                unmarked_numbers_sum -= number;

                auto row_number = card[number].row;
                auto col_number = card[number].col;

                row_marks[row_number]++;
                if (row_marks[row_number] == ROW_SIZE)
                {
                    score = number * unmarked_numbers_sum;
                    return true;
                }

                col_marks[col_number]++;
                if (col_marks[col_number] == COL_SIZE)
                {
                    score = number * unmarked_numbers_sum;
                    return true;
                }
            }

            return false;
        }

        uint64_t get_score()
        {
            return score;
        }

    private:
        struct Position
        {
            uint32_t row;
            uint32_t col;
        };

        std::unordered_map<uint32_t, Position> card;

        uint64_t unmarked_numbers_sum{0};

        uint64_t score{0};

        std::unordered_map<uint32_t, uint32_t> row_marks;

        std::unordered_map<uint32_t, uint32_t> col_marks;
    };
}

void puzzle_04_1()
{
    std::ifstream in_file("/Users/josmil17/Programming/advent21/Advent2021/Advent2021/Puzzle04/input_0.txt");
    if (!in_file)
    {
        std::cerr << "Cannot open file";
        return;
    }

    // Parse input
    std::string line;

    // First line is marked list of numbers
    std::getline(in_file, line);
    std::stringstream ss(line);
    std::vector<uint32_t> marked_numbers;
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        marked_numbers.push_back(std::stoi(substr));
    }

    // Fill in Bingo cards
    std::vector<BingoCard> cards;
    while (std::getline(in_file, line))
    {
        if (line.size() > 0)
        {
            //std::cout << "Adding card" << std::endl;
            BingoCard new_card;
            for (uint32_t row = 0; row < ROW_SIZE; row++)
            {
                std::stringstream ss{line};
                uint32_t number;
                //std::cout << line << std::endl;
                for (uint32_t col = 0; col < COL_SIZE; col++)
                {
                    ss >> number;
                    //std::cout << "Number: " << number << std::endl;
                    new_card.add_number(number, row, col);
                }
                std::getline(in_file, line);
            }
            cards.push_back(new_card);
        }
    }

    // Call numbers and determine winner
    bool won = false;
    for (auto &number : marked_numbers)
    {
        if (won)
        {
            break;
        }
        else
        {
            std::cout << "Calling " << number << std::endl;
            for (uint32_t c = 0; c < cards.size(); c++)
            {
                won = cards[c].mark_number(number);

                if (won)
                {
                    std::cout << "Bingo! Card: " << c << " Score: " << cards[c].get_score() << std::endl;
                    break;
                }
            }
        }
    }

    in_file.close();
}
