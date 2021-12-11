#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Coordinate {
  size_t row    = 0;
  size_t column = 0;

  constexpr bool operator==(const Coordinate &) const = default;

  constexpr bool operator<(const Coordinate &other) const {
    return (row < other.row || (row == other.row && column < other.column));
  }
  constexpr bool operator>(const Coordinate &other) const {
    return (row > other.row || (row == other.row && column > other.column));
  }

  constexpr bool operator<=(const Coordinate &other) const { return !(*this > other); }
  constexpr bool operator>=(const Coordinate &other) const { return !(*this < other); }


  friend std::ostream &operator<<(std::ostream &os, const Coordinate &b) {
    os << "(" << b.row << "," << b.column << ")";
    return os;
  }
};

struct Field {
  size_t width{};
  size_t height{};

  std::vector<std::vector<int>> elements{};

  auto &      operator[](Coordinate c) { return elements[c.row][c.column]; }
  const auto &operator[](Coordinate c) const { return elements[c.row][c.column]; }

  auto get_adjacents(Coordinate c) const {
    std::set<Coordinate> adjacents{};

    const size_t row_begin    = (c.row > 0) ? c.row - 1 : c.row;
    const size_t row_end      = (c.row == height - 1) ? height : c.row + 2;
    const size_t column_begin = (c.column > 0) ? c.column - 1 : c.column;
    const size_t column_end   = (c.column == width - 1) ? width : c.column + 2;

    for (size_t row = row_begin; row < row_end; row++) {
      for (size_t column = column_begin; column < column_end; column++) {
        adjacents.insert({row, column});
      }
    }

    return adjacents;
  }

  constexpr auto size() const { return width * height; }

  friend std::ostream &operator<<(std::ostream &os, const Field &b) {
    for (const auto &row : b.elements) {
      for (const auto &e : row) {
        os << e;
      }
      os << std::endl;
    }
    return os;
  }
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

std::vector<int> get_numbers_from_line(std::string_view line) {
  std::vector<int> out{};

  std::transform(line.begin(), line.end(), std::back_inserter(out), [](char c) {
    return static_cast<int>(c - '0');
  });

  return out;
}

auto get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  Field out{};

  std::transform(
      lines.begin(), lines.end(), std::back_inserter(out.elements), get_numbers_from_line);

  out.height = out.elements.size();
  out.width  = out.elements.front().size();

  return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &v) {
  os << "{ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "}";
  return os;
}

template <typename T>
std::set<T> operator-(const std::set<T> &lhs, const std::set<T> &rhs) {
  std::set<T> out{};

  std::set_difference(
      lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(out, out.begin()));

  return out;
}

template <typename T>
std::set<T> operator+(const std::set<T> &lhs, const std::set<T> &rhs) {
  std::set<T> out{};

  std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(out, out.begin()));

  return out;
}

void increment_and_flash_if_able(Field &field, Coordinate seed, std::set<Coordinate> &flashers) {
  field[seed]++;
  if ((field[seed] > 9) && !flashers.contains(seed)) {
    flashers.insert(seed);

    const auto adjacents = field.get_adjacents(seed);

    // std::cout << seed << " --> " << adjacents << std::endl;

    std::for_each(adjacents.begin(), adjacents.end(), [&](auto c) {
      increment_and_flash_if_able(field, c, flashers);
    });
  }
}

size_t step_and_count_flashes(Field &field) {

  std::set<Coordinate> flashers{};

  for (size_t row = 0; row < field.height; row++) {
    for (size_t column = 0; column < field.width; column++) {
      increment_and_flash_if_able(field, {row, column}, flashers);
    }
  }

  std::for_each(flashers.begin(), flashers.end(), [&](auto c) { field[c] = 0; });

  return flashers.size();
}

int main() {
  auto field = get_game_input_from_file("input.txt");

  int step = 1;
  while(true) {
    const auto new_flashes = step_and_count_flashes(field);

    if(new_flashes == field.size()) break;
    step++;
  }

  const auto solution = step;
  std::cout << solution << std::endl;
}