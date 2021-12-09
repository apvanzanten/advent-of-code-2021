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
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Coordinate {
  size_t row    = 0;
  size_t column = 0;

  constexpr bool operator==(const Coordinate &) const = default;

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

  auto get_adjacent_values(Coordinate c) const {
    std::vector<int> values{};

    if (c.row > 0) values.push_back(elements[c.row - 1][c.column]);
    if (c.column > 0) values.push_back(elements[c.row][c.column - 1]);
    if (c.row < height - 1) values.push_back(elements[c.row + 1][c.column]);
    if (c.column < width - 1) values.push_back(elements[c.row][c.column + 1]);

    return values;
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

void print(const Field &field) {
  for (const auto &row : field.elements) {
    for (const auto &n : row) {
      std::cout << static_cast<int>(n);
    }
    std::cout << std::endl;
  }
}

template <typename T>
void print(const std::vector<T> &v) {
  std::cout << "[ ";
  for (const auto &e : v) {
    std::cout << e << " ";
  }
  std::cout << "]" << std::endl;
}

int main() {
  const auto field = get_game_input_from_file("input.txt");

  std::vector<Coordinate> low_points{};

  for (size_t column = 0; column < field.width; column++) {
    for (size_t row = 0; row < field.height; row++) {
      const Coordinate c{row, column};

      const auto adjacents    = field.get_adjacent_values(c);
      const auto min_adjacent = *std::min_element(adjacents.begin(), adjacents.end());

      if (field[c] < min_adjacent) low_points.push_back(c);
    }
  }


  std::vector<int> risk_levels{};
  
  std::transform(low_points.begin(),
                 low_points.end(),
                 std::back_inserter(risk_levels),
                 [&](Coordinate c) { return field[c] + 1; });

  const auto solution = std::accumulate(risk_levels.begin(), risk_levels.end(), 0);

  std::cout << solution << std::endl;
}