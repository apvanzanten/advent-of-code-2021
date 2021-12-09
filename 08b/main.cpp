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

struct InputLine {
  std::vector<std::set<char>> patterns{};
  std::vector<std::set<char>> outputs{};
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
    const auto pattern_words                     = get_words(patterns_str);
    const auto output_words                      = get_words(output_values_str);

    InputLine il{};
    std::transform(pattern_words.begin(),
                   pattern_words.end(),
                   std::back_inserter(il.patterns),
                   [](const auto &w) {
                     return std::set<char>{w.begin(), w.end()};
                   });

    std::transform(output_words.begin(),
                   output_words.end(),
                   std::back_inserter(il.outputs),
                   [](const auto &w) {
                     return std::set<char>{w.begin(), w.end()};
                   });

    return il;
  });

  return input;
}

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

const std::array<std::vector<size_t>, 8> candidates_by_size{
    std::vector<size_t>{},        // 0
    std::vector<size_t>{},        // 1
    std::vector<size_t>{1},       // 2
    std::vector<size_t>{7},       // 3
    std::vector<size_t>{4},       // 4
    std::vector<size_t>{2, 3, 5}, // 5
    std::vector<size_t>{0, 6, 9}, // 6
    std::vector<size_t>{8}        // 7
};

size_t to_index(char c) { return static_cast<size_t>(c - 'a'); }
char   to_char(size_t i) { return static_cast<char>(i + 'a'); }

void print(const std::set<char> &set) {
  std::cout << "{ ";
  for (const auto &s : set) {
    std::cout << s << " ";
  }
  std::cout << "}";
}

template <size_t N>
void print(const std::array<std::set<char>, N> &keys) {
  for (size_t i = 0; i < keys.size(); i++) {
    std::cout << i << ": ";
    print(keys[i]);
    std::cout << std::endl;
  }
}

int main() {
  const auto inputs = get_game_input_from_file("input.txt");

  size_t solution = 0;

  for (const auto &[patterns, outputs] : inputs) {
    std::array<std::set<char>, 10> digit_keys{};
    std::array<std::set<char>, 7>  segment_keys{};

    // get keys for digits we know by size
    for (const auto &p : patterns) {
      const auto candidates = candidates_by_size[p.size()];

      if (candidates.size() == 1) { digit_keys[candidates.front()] = p; }
    }

    // 7 - 1 -> a (top segment)
    // P.S. C++ is not good at set operations
    std::set_difference(
        digit_keys[7].begin(),
        digit_keys[7].end(),
        digit_keys[1].begin(),
        digit_keys[1].end(),
        std::inserter(segment_keys[to_index('a')], segment_keys[to_index('a')].begin()));

    // 8 - 4 - a -> eg
    std::set<char> eg{};
    std::set_difference(digit_keys[8].begin(),
                        digit_keys[8].end(),
                        digit_keys[4].begin(),
                        digit_keys[4].end(),
                        std::inserter(eg, eg.begin()));
    eg.erase(*segment_keys[to_index('a')].begin());

    // 4 - 1 -> bd
    std::set<char> bd{};
    std::set_difference(digit_keys[4].begin(),
                        digit_keys[4].end(),
                        digit_keys[1].begin(),
                        digit_keys[1].end(),
                        std::inserter(bd, bd.begin()));

    std::set<char> abdeg{};
    std::set_union(bd.begin(), bd.end(), eg.begin(), eg.end(), std::inserter(abdeg, abdeg.begin()));
    abdeg.insert(*segment_keys[to_index('a')].begin());

    // find 3 by removing abdeg (reverse 3) from patterns of length 5, and checking the remainder is
    // equal to digit_keys[1]
    for (const auto &p : patterns) {
      if (p.size() == 5) {
        std::set<char> s{};
        std::set_difference(
            p.begin(), p.end(), abdeg.begin(), abdeg.end(), std::inserter(s, s.begin()));

        if (s == digit_keys[1]) { digit_keys[3] = p; }
      }
    }

    // find b by removing 3 from bd
    segment_keys[to_index('b')].clear();
    std::set_difference(
        bd.begin(),
        bd.end(),
        digit_keys[3].begin(),
        digit_keys[3].end(),
        std::inserter(segment_keys[to_index('b')], segment_keys[to_index('b')].begin()));

    // find e by removing 3 from eg
    segment_keys[to_index('e')].clear();
    std::set_difference(
        eg.begin(),
        eg.end(),
        digit_keys[3].begin(),
        digit_keys[3].end(),
        std::inserter(segment_keys[to_index('e')], segment_keys[to_index('e')].begin()));

    // find g by removing e from eg
    segment_keys[to_index('g')].clear();
    std::set_difference(
        eg.begin(),
        eg.end(),
        segment_keys[to_index('e')].begin(),
        segment_keys[to_index('e')].end(),
        std::inserter(segment_keys[to_index('g')], segment_keys[to_index('g')].begin()));

    // find d by removing b from bd
    segment_keys[to_index('d')].clear();
    std::set_difference(
        bd.begin(),
        bd.end(),
        segment_keys[to_index('b')].begin(),
        segment_keys[to_index('b')].end(),
        std::inserter(segment_keys[to_index('d')], segment_keys[to_index('d')].begin()));

    // create 0 from abeg and 1
    std::set<char> ab{};
    std::set<char> abeg{};
    std::set_union(segment_keys[to_index('a')].begin(),
                   segment_keys[to_index('a')].end(),
                   segment_keys[to_index('b')].begin(),
                   segment_keys[to_index('b')].end(),
                   std::inserter(ab, ab.begin()));
    std::set_union(ab.begin(), ab.end(), eg.begin(), eg.end(), std::inserter(abeg, abeg.begin()));
    std::set_union(abeg.begin(),
                   abeg.end(),
                   digit_keys[1].begin(),
                   digit_keys[1].end(),
                   std::inserter(digit_keys[0], digit_keys[0].begin()));

    // create 9 by removing e from 8
    std::set_difference(digit_keys[8].begin(),
                        digit_keys[8].end(),
                        segment_keys[to_index('e')].begin(),
                        segment_keys[to_index('e')].end(),
                        std::inserter(digit_keys[9], digit_keys[9].begin()));

    // find 6 by checking for length 6 and not 9 or 0
    for (const auto &p : patterns) {
      if (p.size() == 6 && p != digit_keys[9] && p != digit_keys[0]) {
        digit_keys[6] = p;
        break;
      }
    }

    // create 5 by removing e from 6
    std::set_difference(digit_keys[6].begin(),
                        digit_keys[6].end(),
                        segment_keys[to_index('e')].begin(),
                        segment_keys[to_index('e')].end(),
                        std::inserter(digit_keys[5], digit_keys[5].begin()));

    // find f by removing abdeg (reverse 3) from 5
    std::set_difference(
        digit_keys[5].begin(),
        digit_keys[5].end(),
        abdeg.begin(),
        abdeg.end(),
        std::inserter(segment_keys[to_index('f')], segment_keys[to_index('f')].begin()));

    // create 2 by removing bf from 8
    std::set<char> bf{};
    std::set_union(segment_keys[to_index('b')].begin(),
                   segment_keys[to_index('b')].end(),
                   segment_keys[to_index('f')].begin(),
                   segment_keys[to_index('f')].end(),
                   std::inserter(bf, bf.begin()));
    std::set_difference(
        digit_keys[8].begin(),
        digit_keys[8].end(),
        bf.begin(),
        bf.end(),
        std::inserter(digit_keys[2], digit_keys[2].begin()));

    // print(digit_keys);
    // print(segment_keys);

    // decode output
    size_t output = 0;
    size_t magnitude = 1;
    for(int i = outputs.size() - 1; i >= 0; i--) {
      const auto digit = std::distance(digit_keys.begin(), std::find(digit_keys.begin(), digit_keys.end(), outputs[i]));
      output += magnitude * digit;
      magnitude *= 10;
    }
    solution += output;
  }

  std::cout << solution << std::endl;
}