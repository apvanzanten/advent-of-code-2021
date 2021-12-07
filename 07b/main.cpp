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

std::vector<size_t> get_numbers_from_line(const std::string &line) {
  std::istringstream stream{line};
  return std::vector<size_t>{std::istream_iterator<size_t>{stream},
                             std::istream_iterator<size_t>{}};
}

std::string replace_commas_by_spaces(const std::string &line) {
  std::string copy = line;
  std::replace(copy.begin(), copy.end(), ',', ' ');
  return copy;
}

std::vector<size_t> get_game_input_from_file(std::string_view filename) {
  std::fstream file(filename.data(), std::ios_base::in);
  std::string  file_line{};

  std::getline(file, file_line);

  return get_numbers_from_line(replace_commas_by_spaces(file_line));
}

constexpr size_t get_fuel_consumption(size_t distance) {
  size_t fuel = 0;
  for (size_t d = 0; d <= distance; d++) {
    fuel += d;
  }
  return fuel;
}

int main() {
  auto positions = get_game_input_from_file("input.txt");

  const auto [min_it, max_it] = std::minmax_element(positions.begin(), positions.end());
  const size_t min = *min_it, max = *max_it;

  size_t target_pos       = -1;
  size_t fuel_consumption = std::numeric_limits<size_t>::max();

  for (size_t pos = min; pos < max; pos++) {
    const auto sum = std::accumulate(
        positions.begin(), positions.end(), size_t{0}, [=](size_t s, size_t crab_pos) {
          return s + get_fuel_consumption(std::max(pos, crab_pos) - std::min(pos, crab_pos));
        });

    if (sum < fuel_consumption) {
      fuel_consumption = sum;
      target_pos       = pos;
    }
  }

  std::cout << target_pos << "," << fuel_consumption << std::endl;

  const auto solution = fuel_consumption;

  std::cout << solution << std::endl;
}