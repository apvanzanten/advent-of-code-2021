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
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

//   0:      1:      2:      3:      4:
//  aaaa    ....    aaaa    aaaa    ....
// b    c  .    c  .    c  .    c  b    c
// b    c  .    c  .    c  .    c  b    c
//  ....    ....    dddd    dddd    dddd
// e    f  .    f  e    .  .    f  .    f
// e    f  .    f  e    .  .    f  .    f
//  gggg    ....    gggg    gggg    ....

//   5:      6:      7:      8:      9:
//  aaaa    aaaa    aaaa    aaaa    aaaa
// b    .  b    .  .    c  b    c  b    c
// b    .  b    .  .    c  b    c  b    c
//  dddd    dddd    ....    dddd    dddd
// .    f  e    f  .    f  e    f  .    f
// .    f  e    f  .    f  e    f  .    f
//  gggg    gggg    ....    gggg    gggg

const std::array<std::vector<int>, 8> candidates_by_count{
    std::vector<int>{},        // 0
    std::vector<int>{},        // 1
    std::vector<int>{1},       // 2
    std::vector<int>{7},       // 3
    std::vector<int>{4},       // 4
    std::vector<int>{2, 3, 5}, // 5
    std::vector<int>{0, 6, 9}, // 6
    std::vector<int>{8}        // 7
};

struct InputLine {
  std::vector<std::string> patterns{};
  std::vector<std::string> output_values{};
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

std::tuple<std::string_view, std::string_view> split_line(std::string_view line) {
  return std::make_tuple(line.substr(0, line.find('|')), line.substr(line.find('|') + 1));
}

std::vector<std::string> get_words(std::string_view line) {
  std::vector<std::string> words{};
  size_t                   pos = 0;

  while (pos < line.size()) {
    const size_t next_space = line.find(' ', pos);
    if (next_space != line.npos) {
      if (next_space != pos) { words.emplace_back(line.substr(pos, next_space - pos)); }
      pos = next_space + 1;
    } else {
      words.emplace_back(line.substr(pos));
      break;
    }
  }

  return words;
}

std::vector<InputLine> get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  std::vector<InputLine> input{};

  std::transform(lines.begin(), lines.end(), std::back_inserter(input), [](auto line) {
    const auto [patterns_str, output_values_str] = split_line(line);
    return InputLine{get_words(patterns_str), get_words(output_values_str)};
  });

  return input;
}

int main() {
  const auto inputs = get_game_input_from_file("input.txt");

  const auto solution =
      std::accumulate(inputs.begin(), inputs.end(), size_t{0}, [](auto sum, const auto &i) {
        return sum + std::count_if(i.output_values.begin(), i.output_values.end(), [](auto o) {
                 return (candidates_by_count[o.size()].size() == 1);
               });
      });

  std::cout << solution << std::endl;
}