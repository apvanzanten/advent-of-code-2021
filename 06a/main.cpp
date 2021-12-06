#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
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

struct Fish {
  int8_t timer = 8;

  constexpr bool operator==(const Fish &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Fish &f) {
    os << static_cast<int>(f.timer);
    return os;
  }
};

constexpr void cycle(Fish &f) {
  f.timer--;
  if (f.timer < 0) f.timer = 6;
}

std::vector<int> get_numbers_from_line(const std::string &line) {
  std::istringstream stream{line};
  return std::vector<int>{std::istream_iterator<int>{stream}, std::istream_iterator<int>{}};
}

std::string replace_commas_by_spaces(const std::string &line) {
  std::string copy = line;
  std::replace(copy.begin(), copy.end(), ',', ' ');
  return copy;
}

std::deque<Fish> get_game_input_from_file(std::string_view filename) {
  std::fstream file(filename.data(), std::ios_base::in);
  std::string  file_line{};

  std::getline(file, file_line);

  const auto numbers = get_numbers_from_line(replace_commas_by_spaces(file_line));

  std::deque<Fish> fishes{};

  std::transform(numbers.begin(), numbers.end(), std::back_inserter(fishes), [](int n) {
    return Fish{static_cast<int8_t>(n)};
  });

  return fishes;
}

int main() {
  auto fishes = get_game_input_from_file("input.txt");

  for (int day = 0; day < 80; day++) {
    // count fishes that are about to reproduce
    const size_t num_new_fishes = std::count(fishes.begin(), fishes.end(), Fish{0});

    // cycle all current fishes
    std::for_each(fishes.begin(), fishes.end(), cycle);

    // add new fishes
    std::generate_n(std::back_inserter(fishes), num_new_fishes, []{ return Fish{}; });
  }

  const auto solution = fishes.size();
  
  std::cout << solution << std::endl;
}