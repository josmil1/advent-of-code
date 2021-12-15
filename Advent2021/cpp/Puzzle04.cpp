#include "Puzzles.hpp"

#define ROW_SIZE 5
#define COL_SIZE 5

namespace {
class BingoCard {
public:
  /**
   * @brief For initialisation: add number to the
   * bingo card
   */
  void add_number(uint32_t number, uint32_t row, uint32_t col) {
    card[number] = {row, col};

    // std::cout << "Added at " << card[number].row << ", " << card[number].col
    // << std::endl;

    unmarked_numbers_sum += number;
  }

  /**
   * @brief Return true if the new number results
   * in a win (completed row/col)
   */
  bool mark_number(uint32_t number) {
    if (card.count(number)) {
      // Number in card
      unmarked_numbers_sum -= number;

      auto row_number = card[number].row;
      auto col_number = card[number].col;

      row_marks[row_number]++;
      if (row_marks[row_number] == ROW_SIZE) {
        score = number * unmarked_numbers_sum;
        return true;
      }

      col_marks[col_number]++;
      if (col_marks[col_number] == COL_SIZE) {
        score = number * unmarked_numbers_sum;
        return true;
      }
    }

    return false;
  }

  uint64_t get_score() { return score; }

  void set_id(uint64_t _id) { id = _id; }

  uint64_t get_id() { return id; }

private:
  struct Position {
    uint32_t row;
    uint32_t col;
  };

  std::unordered_map<uint32_t, Position> card;

  uint64_t unmarked_numbers_sum{0};

  uint64_t score{0};

  uint64_t id{0};

  std::unordered_map<uint32_t, uint32_t> row_marks;

  std::unordered_map<uint32_t, uint32_t> col_marks;
};

uint64_t puzzle_04_part(std::ifstream &in_file, bool part1) {
  // Parse input
  std::string line;

  // First line is marked list of numbers
  std::getline(in_file, line);
  std::stringstream ss(line);
  std::vector<uint32_t> marked_numbers;
  while (ss.good()) {
    std::string substr;
    std::getline(ss, substr, ',');
    marked_numbers.push_back(std::stoul(substr));
  }

  // Fill in Bingo cards
  std::vector<BingoCard> cards;
  uint64_t card_count = 0;
  while (std::getline(in_file, line)) {
    if (line.size() > 0) {
      // std::cout << "Adding card" << std::endl;
      BingoCard new_card;
      new_card.set_id(card_count);
      for (uint32_t row = 0; row < ROW_SIZE; row++) {
        ss = std::stringstream(line);
        uint32_t number;
        // std::cout << line << std::endl;
        for (uint32_t col = 0; col < COL_SIZE; col++) {
          ss >> number;
          // std::cout << "Number: " << number << std::endl;
          new_card.add_number(number, row, col);
        }
        std::getline(in_file, line);
      }
      cards.push_back(new_card);
      card_count++;
    }
  }

  // Call numbers and determine winner
  bool won = false;
  uint64_t card_id = 0;
  for (auto &number : marked_numbers) {
    auto it = cards.begin();
    while (it != cards.end()) {
      won = it->mark_number(number);
      if (won) {
        card_id = it->get_id();
        std::cout << "Bingo! Card: " << card_id << " Score: " << it->get_score()
                  << std::endl;
        it = cards.erase(it);
      } else {
        ++it;
      }
    }
  }

  return part1 ? card_id : card_id;
}
} // namespace

uint64_t puzzle_04(std::ifstream &in_file) {
  return puzzle_04_part(in_file, false);
}
