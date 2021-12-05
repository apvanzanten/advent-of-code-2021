#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Coordinate {
  int x = 0;
  int y = 0;

  constexpr bool operator==(const Coordinate &) const = default;
};

struct Line {
  Coordinate begin{};
  Coordinate end{};

  constexpr bool operator==(const Line &) const = default;

  std::vector<Coordinate> points() const {
    // NOTE assumes line is either vertical or horizontal
    const int delta_x = std::clamp((end.x - begin.x), -1, 1);
    const int delta_y = std::clamp((end.y - begin.y), -1, 1);

    std::vector<Coordinate> p{};

    for (Coordinate c = begin; c != end; c.x += delta_x, c.y += delta_y) {
      p.push_back(c);
    }

    p.push_back(end);

    return p;
  }

  constexpr int length() const {
    // NOTE assumes line is either vertical or horizontal
    return std::max(std::abs(end.x - begin.x), std::abs(end.y - begin.y));
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

Line parse_line(const std::string &str) {
  const auto first_comma_idx  = str.find(",");
  const auto first_space_idx  = str.find(" ", first_comma_idx + 1);
  const auto second_space_idx = str.find(" ", first_space_idx + 1);
  const auto second_comma_idx = str.find(",", second_space_idx + 1);

  const auto begin_x_str = str.substr(0, first_comma_idx);
  const auto begin_y_str = str.substr(first_comma_idx + 1, first_space_idx);
  const auto end_x_str   = str.substr(second_space_idx + 1, second_comma_idx);
  const auto end_y_str   = str.substr(second_comma_idx + 1);

  return {{std::stoi(begin_x_str), std::stoi(begin_y_str)},
          {std::stoi(end_x_str), std::stoi(end_y_str)}};
}

std::vector<Line> get_game_input_from_file(std::string_view filename) {
  const auto file_lines = get_non_empty_lines_from_file(filename);

  std::vector<Line> lines{};
  lines.reserve(file_lines.size());

  std::transform(file_lines.begin(), file_lines.end(), std::back_inserter(lines), parse_line);

  return lines;
}

std::tuple<Coordinate, Coordinate> get_boundaries(const std::vector<Line> &lines) {
  // start with begin/end of first line, s.t. we always have a valid value
  Coordinate top_left     = lines.front().begin;
  Coordinate bottom_right = lines.front().end;

  for (const auto &line : lines) {
    top_left.x     = std::min({top_left.x, line.begin.x, line.end.x});
    top_left.y     = std::min({top_left.y, line.begin.y, line.end.y});
    bottom_right.x = std::max({bottom_right.x, line.begin.x, line.end.x});
    bottom_right.y = std::max({bottom_right.y, line.begin.y, line.end.y});
  }

  return std::make_tuple(top_left, bottom_right);
}

template <>
struct std::hash<Coordinate> {
  std::size_t operator()(const Coordinate &coord) const noexcept {
    auto x = std::hash<int>{}(coord.x);
    auto y = std::hash<int>{}(coord.y);
    return (x ^ (y << 1));
  }
};

int main() {
  auto lines = get_game_input_from_file("input.txt");

  // filter out diagonal lines
  lines.erase(std::remove_if(lines.begin(),
                             lines.end(),
                             [](auto line) {
                               return (line.begin.x != line.end.x) && (line.begin.y != line.end.y);
                             }),
              lines.end());

  // create and populate vent map
  std::unordered_map<Coordinate, uint8_t> vent_map{};

  for (const auto &line : lines) {
    for (const auto &p : line.points()) {
      if (!vent_map.contains(p)) vent_map[p] = 0;
      vent_map[p]++;
    }
  }

  // eliminate points in vent_map where there is only 1 line
  std::erase_if(vent_map, [](const auto &pair) {
    const auto &[coord, count] = pair;
    return count < 2;
  });

  const auto solution = vent_map.size();

  std::cout << solution << std::endl;
}