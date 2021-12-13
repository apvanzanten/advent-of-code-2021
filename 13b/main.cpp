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
  int x = 0;
  int y = 0;

  constexpr bool operator==(const Coordinate &) const = default;

  constexpr bool operator<(const Coordinate &other) const {
    return (y < other.y || (y == other.y && x < other.x));
  }
  constexpr bool operator>(const Coordinate &other) const {
    return (y > other.y || (y == other.y && x > other.x));
  }
  constexpr bool operator<=(const Coordinate &other) const { return !(*this > other); }
  constexpr bool operator>=(const Coordinate &other) const { return !(*this < other); }

  static Coordinate from_line(std::string_view line) {
    const auto delim_idx = line.find(',');
    return Coordinate{std::stoi(line.substr(0, delim_idx).data()),
                      std::stoi(line.substr(delim_idx + 1).data())};
  }

  static Coordinate from_fold_line(std::string_view line) {
    const bool is_x       = line.starts_with("fold along x");
    const auto equals_idx = line.find('=');
    const int  num        = std::stoi(line.substr(equals_idx + 1).data());

    return (is_x ? Coordinate{num, 0} : Coordinate{0, num});
  }

  friend std::ostream &operator<<(std::ostream &os, const Coordinate &c) {
    os << "(" << c.x << "," << c.y << ")";
    return os;
  }
};

constexpr Coordinate fold_on(Coordinate c, Coordinate fold) {
  Coordinate out{c};
  if (fold.y > 0 && fold.y < c.y) { out.y = fold.y - (c.y - fold.y); }
  if (fold.x > 0 && fold.x < c.x) { out.x = fold.x - (c.x - fold.x); }
  return out;
}

auto get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  std::vector<Coordinate> points{};
  std::vector<Coordinate> folds{};

  const auto fold_it =
      std::find_if(lines.begin(), lines.end(), [](auto str) { return str[0] == 'f'; });

  std::transform(lines.begin(), fold_it, std::back_inserter(points), Coordinate::from_line);

  std::transform(fold_it, lines.end(), std::back_inserter(folds), Coordinate::from_fold_line);

  std::sort(points.begin(), points.end());

  return std::make_tuple(points, folds);
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

void print(const std::set<Coordinate> & points) {
  const auto max_x_it = std::max_element(points.begin(), points.end(), [](auto a, auto b) { return a.x < b.x; });
  const auto max_y_it = std::max_element(points.begin(), points.end(), [](auto a, auto b) { return a.y < b.y; });

  for(int y = 0; y <= max_y_it->y; y++) {
    for(int x = 0; x <= max_x_it->x; x++) {
      std::cout << (points.contains(Coordinate{x,y}) ? '#' : '.');
    }
    std::cout << '\n';
  }
}

int main() {
  const auto [points, folds] = get_game_input_from_file("input.txt");

  std::set<Coordinate> folded_points{}; // use a set so we automatically get rid of duplicates

  std::transform(points.begin(),
                 points.end(),
                 std::inserter(folded_points, folded_points.begin()),
                 [&](Coordinate c) {
                   return std::accumulate(
                       folds.begin(), folds.end(), c, [](Coordinate foldee, Coordinate folder) {
                         return fold_on(foldee, folder);
                       });
                 });

  print(folded_points);
}