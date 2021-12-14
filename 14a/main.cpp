#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
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

struct Rule {
  char lhs{};
  char rhs{};
  char to_be_inserted{};

  constexpr bool operator==(const Rule &) const = default;
};

constexpr Rule parse_rule(std::string_view str) {
  return Rule{str[0], str[1], str[str.size() - 1]};
}

auto get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  std::vector<Rule> rules{};

  std::transform(std::next(lines.begin()), lines.end(), std::back_inserter(rules), parse_rule);

  return std::make_tuple(lines[0], rules);
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
std::ostream &operator<<(std::ostream &os, const std::list<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e;
  }
  os << " ]";
  return os;
}

int main() {
  const auto [tmpl, rules] = get_game_input_from_file("input.txt");
  const size_t num_steps   = 10;

  std::list<char> polymer{tmpl.begin(), tmpl.end()};

  for (size_t i = 1; i <= num_steps; i++) {
    auto lhs_it = polymer.begin();
    auto rhs_it = std::next(polymer.begin()); // lets just assume there's at least 2 elements

    while (rhs_it != polymer.end()) {
      for (const auto rule : rules) {
        if (rule.lhs == *lhs_it && rule.rhs == *rhs_it) {
          polymer.insert(rhs_it, rule.to_be_inserted);
        }
      }

      lhs_it = rhs_it; // note if a new element was added, this skips it
      rhs_it++;
    }
  }

  std::map<char, size_t> element_counts{};
  for (const auto element : polymer) {
    if (!element_counts.contains(element)) { element_counts[element] = 0; }
    element_counts[element]++;
  }

  const auto [min_it, max_it] =
      std::minmax_element(element_counts.begin(), element_counts.end(), [](auto lhs, auto rhs) {
        const auto [_lhs, lhs_count] = lhs;
        const auto [_rhs, rhs_count] = rhs;
        return (lhs_count < rhs_count);
      });

  const auto solution = (max_it->second) - (min_it->second);

  std::cout << solution << std::endl;
}