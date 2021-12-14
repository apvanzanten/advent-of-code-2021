#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
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

auto get_element_counts(const std::map<std::pair<char, char>, size_t> &pair_counts) {
  std::map<char, size_t> counts{};

  for (const auto &[pair, count] : pair_counts) {
    if (!counts.contains(pair.first)) counts[pair.first] = 0;
    if (!counts.contains(pair.second)) counts[pair.second] = 0;

    counts[pair.first] += count;
    counts[pair.second] += count;
  }

  // halve the counts to account for pairs containing the same distinct element twice (and round up
  // to deal with start and end of polymer)
  for (auto &[_, count] : counts) {
    count = ((count + 1) / 2);
  }

  return counts;
}

auto get_least_and_most_occurring(const std::map<char, size_t> &element_counts) {
  const auto [min_it, max_it] =
      std::minmax_element(element_counts.begin(), element_counts.end(), [](auto lhs, auto rhs) {
        const auto [_lhs, lhs_count] = lhs;
        const auto [_rhs, rhs_count] = rhs;
        return (lhs_count < rhs_count);
      });

  return std::make_pair(*min_it, *max_it);
}

auto get_pair_counts(const auto &tmpl) {
  std::map<std::pair<char, char>, size_t> pair_counts{};

  for (auto lhs_it = tmpl.begin(); lhs_it != std::prev(tmpl.end()); lhs_it++) {
    const auto lhs = *lhs_it;
    const auto rhs = *std::next(lhs_it);

    if (!pair_counts.contains({lhs, rhs})) pair_counts[{lhs, rhs}] = 0;
    pair_counts[{lhs, rhs}]++;
  }

  return pair_counts;
}

int main() {
  const auto [tmpl, rules] = get_game_input_from_file("input.txt");
  const size_t num_steps   = 40;

  auto pair_counts = get_pair_counts(tmpl);

  for (size_t step = 1; step <= num_steps; step++) {
    std::map<std::pair<char, char>, size_t> new_pair_counts{};
    std::vector<std::pair<char, char>>      removed_pairs{};

    for (const auto &[pair, count] : pair_counts) {
      for (const auto rule : rules) {
        if (rule.lhs == pair.first && rule.rhs == pair.second) {
          const auto first  = std::make_pair(rule.lhs, rule.to_be_inserted);
          const auto second = std::make_pair(rule.to_be_inserted, rule.rhs);

          if (!new_pair_counts.contains(first)) new_pair_counts[first] = 0;
          if (!new_pair_counts.contains(second)) new_pair_counts[second] = 0;

          new_pair_counts[first] += count;
          new_pair_counts[second] += count;

          removed_pairs.push_back(pair);
        }
      }
    }

    for (const auto &pair : removed_pairs) {
      pair_counts.erase(pair);
    }

    for (const auto &[pair, count] : new_pair_counts) {
      if (!pair_counts.contains(pair)) pair_counts[pair] = 0;
      pair_counts[pair] += count;
    }
  }

  const auto [min, max] = get_least_and_most_occurring(get_element_counts(pair_counts));
  const auto solution   = (max.second - min.second);
  std::cout << solution << std::endl;
}