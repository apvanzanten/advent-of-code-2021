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
    std::vector<Coordinate> adjacents{};

    if (c.row > 0) adjacents.push_back(Coordinate{c.row - 1, c.column});
    if (c.column > 0) adjacents.push_back(Coordinate{c.row, c.column - 1});
    if (c.column < width - 1) adjacents.push_back(Coordinate{c.row, c.column + 1});
    if (c.row < height - 1) adjacents.push_back(Coordinate{c.row + 1, c.column});

    return adjacents;
  }

  auto get_adjacent_values(Coordinate c) const {
    std::vector<int> values{};
    const auto       adjacents = get_adjacents(c);

    std::transform(adjacents.begin(), adjacents.end(), std::back_inserter(values), [&](auto c) {
      return (*this)[c];
    });

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
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

void insert_if_not_already_in(std::vector<Coordinate> &v, Coordinate c) {
  // assumes v is sorted
  if (!std::binary_search(v.begin(), v.end(), c)) {
    v.push_back(c);
    // ensure v remains sorted
    std::sort(v.begin(), v.end());
  }
}

std::vector<Coordinate> get_low_points(const Field &field) {
  std::vector<Coordinate> low_points{};

  for (size_t column = 0; column < field.width; column++) {
    for (size_t row = 0; row < field.height; row++) {
      const Coordinate c{row, column};

      const auto adjacents    = field.get_adjacent_values(c);
      const auto min_adjacent = *std::min_element(adjacents.begin(), adjacents.end());

      if (field[c] < min_adjacent) low_points.push_back(c);
    }
  }

  return low_points;
}

std::vector<Coordinate> grow_basin_from(const Field &field, Coordinate seed) {
  std::vector<Coordinate> basin{seed};

  while (true) {
    std::vector<Coordinate> new_basin{basin.begin(), basin.end()};

    for (const auto &c : basin) {
      for (const auto &a : field.get_adjacents(c)) {
        if (field[a] < 9 && field[a] >= field[c]) insert_if_not_already_in(new_basin, a);
      }
    }

    if (new_basin == basin) {
      // fixed point reached, end
      break;
    } else {
      basin = std::move(new_basin);
    }
  };

  return basin;
}

int main() {
  const auto field = get_game_input_from_file("input.txt");

  const auto low_points = get_low_points(field);

  std::vector<std::vector<Coordinate>> basins{};
  std::transform(low_points.begin(), low_points.end(), std::back_inserter(basins), [&](auto c) {
    return grow_basin_from(field, c);
  });

  std::vector<size_t> basin_sizes{};
  std::transform(basins.begin(),
                 basins.end(),
                 std::back_inserter(basin_sizes),
                 [](const auto &basin) { return basin.size(); });

  std::partial_sort(
      basin_sizes.begin(), std::next(basin_sizes.begin(), 3), basin_sizes.end(), std::greater{});

  const auto solution = std::accumulate(basin_sizes.begin(),
                                        std::next(basin_sizes.begin(), 3),
                                        size_t{1},
                                        [](auto product, auto size) { return product * size; });

  std::cout << solution << std::endl;
}