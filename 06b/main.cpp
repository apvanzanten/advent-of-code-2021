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

using FishTimerHistogram = std::array<size_t, 9>;

std::vector<int> get_numbers_from_line(const std::string &line) {
  std::istringstream stream{line};
  return std::vector<int>{std::istream_iterator<int>{stream}, std::istream_iterator<int>{}};
}

std::string replace_commas_by_spaces(const std::string &line) {
  std::string copy = line;
  std::replace(copy.begin(), copy.end(), ',', ' ');
  return copy;
}

FishTimerHistogram get_game_input_from_file(std::string_view filename) {
  std::fstream file(filename.data(), std::ios_base::in);
  std::string  file_line{};

  std::getline(file, file_line);

  const auto numbers = get_numbers_from_line(replace_commas_by_spaces(file_line));

  FishTimerHistogram hist{};

  for(size_t i = 0; i < hist.size(); i++) {
    hist[i] = std::count(numbers.begin(), numbers.end(), static_cast<int>(i));
  }

  return hist;
}

int main() {
  auto hist = get_game_input_from_file("input.txt");

  for (int day = 0; day < 256; day++) {
    // OMG it's just a rotate followed by some adding! Thanks, Sean Parent <3

    std::rotate(hist.begin(), std::next(hist.begin()), hist.end());

    hist[6] += hist[8];
  }

  // count up all fishes
  const auto solution = std::accumulate(hist.begin(), hist.end(), size_t{0});

  std::cout << solution << std::endl;
}