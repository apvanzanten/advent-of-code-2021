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

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}
std::vector<std::string> get_non_empty_lines_from_file(std::string_view filename) {
  std::fstream             file(filename.data(), std::ios_base::in);
  std::vector<std::string> lines{};

  std::string line{};
  while (std::getline(file, line)) {
    if (!line.empty()) lines.push_back(line);
  }

  return lines;
}

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

struct Cavern {
  size_t width{};
  size_t height{};

  std::vector<std::vector<int>> risk_levels{};

  auto &      operator[](Coordinate c) { return risk_levels[c.row][c.column]; }
  const auto &operator[](Coordinate c) const { return risk_levels[c.row][c.column]; }

  auto get_adjacents(Coordinate c) const {
    std::vector<Coordinate> adjacents{};

    if (c.column < width - 1) adjacents.emplace_back(c.row, c.column + 1);
    if (c.row < height - 1) adjacents.emplace_back(c.row + 1, c.column);
    if (c.column > 0) adjacents.emplace_back(c.row, c.column - 1);
    if (c.row > 0) adjacents.emplace_back(c.row - 1, c.column);

    return adjacents;
  }

  friend std::ostream &operator<<(std::ostream &os, const Cavern &b) {
    for (const auto &row : b.risk_levels) {
      for (int l : row) {
        os << l;
      }
      os << std::endl;
    }
    return os;
  }
};

auto get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  Cavern cavern{};
  cavern.width  = lines.front().size();
  cavern.height = lines.size();
  cavern.risk_levels.reserve(cavern.height);

  std::transform(
      lines.begin(), lines.end(), std::back_inserter(cavern.risk_levels), [&](const auto &line) {
        std::vector<int> row{};
        std::transform(line.begin(), line.end(), std::back_inserter(row), [](char c) -> int {
          return (c - '0');
        });
        return row;
      });

  return cavern;
}

void print_risk_sums(const std::vector<std::vector<int>> &sums) {
  for(const auto & s : sums) {
    std::cout << s << std::endl;
  }
}

int main() {
  const auto cavern = get_game_input_from_file("input.txt");

  std::cout << cavern << std::endl;

  std::vector<std::vector<int>> risk_sums(
      cavern.height, std::vector<int>(cavern.width, (std::numeric_limits<int>::max() - 10)));

  risk_sums.back().back() = cavern.risk_levels.back().back(); // set exit point

  bool is_stable = false;

  std::vector<int> sum_candidates{};
  sum_candidates.reserve(4);

  while (!is_stable) {
    // assert is_stable to begin with, then set it to false if we change anything
    is_stable = true;

    for (int row = cavern.height - 1; row >= 0; row--) {
      for (int column = cavern.width - 1; column >= 0; column--) {
        const auto position  = Coordinate{static_cast<size_t>(row), static_cast<size_t>(column)};
        const auto base_risk = cavern[position];
        const auto adjacents = cavern.get_adjacents(position);

        std::transform(adjacents.begin(), adjacents.end(), std::back_inserter(sum_candidates), [&](Coordinate c) {
          return risk_sums[c.row][c.column] + base_risk;
        });

        auto & current_sum = risk_sums[position.row][position.column];
        const auto new_sum = *std::min_element(sum_candidates.begin(), sum_candidates.end());
        if(new_sum < current_sum) {
          current_sum = new_sum;
          is_stable = false;
        }

        sum_candidates.clear();
      }
    }
  }
  
  std::cout << (risk_sums.front().front() - cavern.risk_levels.front().front()) << std::endl;
}