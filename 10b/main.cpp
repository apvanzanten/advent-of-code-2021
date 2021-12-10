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

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

constexpr bool is_opening(char c) { return (c == '(' || c == '<' || c == '{' || c == '['); }

constexpr char get_closer_for(char c) {
  switch (c) {
  case '(': return ')';
  case '[': return ']';
  case '{': return '}';
  case '<': return '>';
  default: throw std::runtime_error("bad character in get_closer_for()");
  }
}

constexpr bool is_closer_for(char c, char opener) { return c == get_closer_for(opener); }

constexpr size_t score_closer(char c) {
  switch (c) {
  case ')': return 1;
  case ']': return 2;
  case '}': return 3;
  case '>': return 4;
  default: throw std::runtime_error("bad character in score()");
  }
}

std::vector<char> complete_line(std::string_view line) {
  std::stack<char> stack{};

  for (char c : line) {
    if (is_opening(c)) {
      stack.push(c);
    } else if (!stack.empty() && is_closer_for(c, stack.top())) {
      stack.pop();
    } else {
      return {};
    }
  }

  std::vector<char> out{};
  std::generate_n(std::back_inserter(out), stack.size(), [&] {
    const auto closer = get_closer_for(stack.top());
    stack.pop();
    return closer;
  });

  return out;
}

int main() {
  const auto lines = get_non_empty_lines_from_file("input.txt");

  std::vector<size_t> scores{};

  // get scores for lines
  std::transform(lines.begin(), lines.end(), std::back_inserter(scores), [](const auto &line) {
    const auto cmp = complete_line(line);
    return std::accumulate(cmp.begin(), cmp.end(), size_t{0}, [](size_t s, char closer) {
      return (s * 5) + score_closer(closer);
    });
  });

  // filter out 0 scores
  scores.erase(std::remove(scores.begin(), scores.end(), 0), scores.end());

  // partition around middle
  auto mid_it = std::next(scores.begin(), (scores.size() / 2));
  std::nth_element(scores.begin(), mid_it, scores.end());

  const auto solution = *mid_it;

  std::cout << solution << std::endl;
}