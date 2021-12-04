#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t board_width  = 5;
constexpr size_t board_height = 5;

struct Board {
  struct Number {
    int  val       = 0;
    bool is_marked = false;

    constexpr Number() = default;
    constexpr Number(int n) : val(n) {}
    constexpr bool operator<=>(const Number &) const = default;
  };

  std::array<Number, board_width * board_height> numbers{};

  constexpr size_t index(size_t row, size_t column) const { return (row * board_width) + column; };

  bool mark_if_present(int n) {
    for (auto &board_n : numbers) {
      if (board_n.val == n) {
        board_n.is_marked = true;
        return true;
      }
    }
    return false;
  }

  constexpr bool has_full_column_or_row() const {
    for (size_t row = 0; row < board_height; row++) {
      bool is_full = true;

      for (size_t column = 0; column < board_width; column++) {
        is_full = (is_full && numbers[index(row, column)].is_marked);
      }

      if (is_full) return true;
    }

    for (size_t column = 0; column < board_width; column++) {
      bool is_full = true;

      for (size_t row = 0; row < board_height; row++) {
        is_full = (is_full && numbers[index(row, column)].is_marked);
      }

      if (is_full) return true;
    }

    return false;
  }

  friend std::ostream &operator<<(std::ostream &os, Board &b) {
    for (auto row_start_it = b.numbers.begin(); row_start_it != b.numbers.end();
         std::advance(row_start_it, board_width)) {
      for (auto column_it = row_start_it; column_it != std::next(row_start_it, board_width);
           column_it++) {
        os << (column_it->is_marked ? '>' : ' ') << column_it->val << " ";
      }
      os << '\n';
    }
    return os;
  }
};

struct GameInput {
  std::vector<int>   drawn_numbers{};
  std::vector<Board> boards{};
};

std::vector<std::string> get_non_empty_lines_from_file(std::string_view filename) {
  std::fstream             file(filename.data(), std::ios_base::in);
  std::vector<std::string> lines{};

  std::string line{};
  while (std::getline(file, line)) {
    if (!line.empty()) lines.push_back(line);
  }

  return lines;
}

std::vector<int> get_numbers_from_line(const std::string &line) {
  std::istringstream stream{line};
  return std::vector<int>{std::istream_iterator<int>{stream}, std::istream_iterator<int>{}};
}

std::vector<Board> get_boards_from_lines(auto begin, auto end) {
  std::vector<Board> boards{};

  auto line_it = begin;
  while (line_it != end) {
    Board current_board{};
    auto  board_row_it = current_board.numbers.begin();

    for (size_t row = 0; (row < board_height) && (line_it != end); row++) {
      const auto numbers_for_row = get_numbers_from_line(*line_it);

      std::transform(numbers_for_row.begin(), numbers_for_row.end(), board_row_it, [](int n) {
        return Board::Number{n};
      });

      std::advance(line_it, 1);
      std::advance(board_row_it, board_width);
    }

    boards.push_back(current_board);
  }

  return boards;
}

std::string replace_commas_by_spaces(const std::string &line) {
  std::string copy = line;
  std::replace(copy.begin(), copy.end(), ',', ' ');
  return copy;
}

GameInput get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  return GameInput{get_numbers_from_line(replace_commas_by_spaces(lines.front())),
                   get_boards_from_lines(std::next(lines.begin()), lines.end())};
}

int main() {
  auto input = get_game_input_from_file("input.txt");

  for (int drawn_n : input.drawn_numbers) {
    std::cout << "drawing for " << input.boards.size() << " boards: " << drawn_n << std::endl;

    if (input.boards.size() > 1) {
      input.boards.erase(std::remove_if(input.boards.begin(),
                                        input.boards.end(),
                                        [=](auto &board) {
                                          return (board.mark_if_present(drawn_n) &&
                                                  board.has_full_column_or_row());
                                        }),
                         input.boards.end());
    } else { // we are on the last board, stop removing
      auto &board = input.boards.front();
      board.mark_if_present(drawn_n);

      if (board.has_full_column_or_row()) {
        const auto final_draw = drawn_n;
        const auto sum_of_unmarked =
            std::accumulate(board.numbers.begin(), board.numbers.end(), 0, [](auto s, auto n) {
              return s + (n.is_marked ? 0 : n.val);
            });

        const auto solution = sum_of_unmarked * final_draw;

        std::cout << solution << std::endl;
        return 0;
      }
    }
  }
}