#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

std::vector<std::string> get_non_empty_lines_from_file(std::string_view filename) {
  std::fstream             file(filename.data(), std::ios_base::in);
  std::vector<std::string> lines{};

  std::string line{};
  while (std::getline(file, line)) {
    if (!line.empty()) lines.push_back(line);
  }

  return lines;
}

struct Vec2D {
  int x = 0;
  int y = 0;

  constexpr bool operator==(const Vec2D &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Vec2D &v) {
    os << "Vec2D{" << v.x << "," << v.y << "}";
    return os;
  }

  constexpr Vec2D operator+(const Vec2D &rhs) const { return {x + rhs.x, y + rhs.y}; }

  constexpr Vec2D operator-(const Vec2D &rhs) const { return {x - rhs.x, y - rhs.y}; }

  constexpr Vec2D &operator+=(const Vec2D &rhs) {
    *this = *this + rhs;
    return *this;
  }

  constexpr Vec2D &operator-=(const Vec2D &rhs) {
    *this = *this - rhs;
    return *this;
  }
};

using Position = Vec2D;
using Velocity = Vec2D;

struct Target {
  Position left_top{};
  Position right_bottom{};

  constexpr bool operator==(const Target &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Target &t) {
    os << "Target{" << t.left_top << "," << t.right_bottom << "}";
    return os;
  }

  constexpr bool contains_x(int x) const { return x >= left_top.x && x <= right_bottom.x; }
  constexpr bool contains_y(int y) const { return y <= left_top.y && y >= right_bottom.y; }
  constexpr bool contains(Position pos) const { return contains_x(pos.x) && contains_y(pos.y); }
  constexpr bool is_right_of(int x) const { return x < left_top.x; }
  constexpr bool is_left_of(int x) const { return x > right_bottom.x; }
  constexpr bool is_below(int y) const { return y > left_top.y; }
  constexpr bool is_above(int y) const { return y < right_bottom.y; }
};

Target parse_target_line(std::string_view line) {
  const auto x_part_begin = line.find('=') + 1;
  const auto x_part_end   = line.find(',');

  const auto x_part = line.substr(x_part_begin, x_part_end - x_part_begin);
  const auto y_part = line.substr(line.rfind('=') + 1);

  const auto left  = x_part.substr(0, x_part.find('.'));
  const auto right = x_part.substr(x_part.rfind('.') + 1);

  const auto bottom = y_part.substr(0, y_part.find('.'));
  const auto top    = y_part.substr(y_part.rfind('.') + 1);

  return {{atoi(left.data()), atoi(top.data())}, {atoi(right.data()), atoi(bottom.data())}};
}

std::vector<Target> get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  std::vector<Target> targets{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(targets), parse_target_line);

  return targets;
}

using Trajectory = std::vector<Position>;

struct Probe {
  Position position{};
  Velocity velocity{};

  constexpr bool operator==(const Probe &) const = default;

  static constexpr Probe launch(Velocity v) { return Probe{{0, 0}, v}; }

  friend std::ostream &operator<<(std::ostream &os, const Probe &p) {
    os << "Probe{" << p.position << "," << p.velocity << "}";
    return os;
  }

  constexpr void step() {
    position += velocity;
    velocity.x -= std::clamp(velocity.x, -1, 1);
    velocity.y -= 1;
  }

  Trajectory shoot_for_target(const Target &t) {
    Trajectory trajectory{};

    while (true) {
      trajectory.push_back(position);

      if (t.contains(position)) break;                         // hit
      if (t.is_right_of(position.x) && velocity.x == 0) break; // not enough x to make it, undershot
      if (t.is_above(position.y)) break;                       // too low, undershot
      if (!t.is_above(position.y) && t.is_left_of(position.x)) break; // too far right, overshot

      step();
    }
    return trajectory;
  }
};

void print_trajectory_and_target(const Trajectory &traj, const Target &target) {
  const auto [left_it, right_it] = std::minmax_element(
      traj.begin(), traj.end(), [](auto pos_a, auto pos_b) { return pos_a.x < pos_b.x; });

  const auto [bottom_it, top_it] = std::minmax_element(
      traj.begin(), traj.end(), [](auto pos_a, auto pos_b) { return pos_a.y < pos_b.y; });

  for (int y = top_it->y; y >= bottom_it->y; y--) {
    std::cout << std::setw(4) << y << " ";
    for (int x = left_it->x; x <= right_it->x; x++) {
      if (x == 0 && y == 0) {
        std::cout << 'S';
      } else if (std::find(traj.begin(), traj.end(), Position{x, y}) != traj.end()) {
        std::cout << '#';
      } else if (target.contains({x, y})) {
        std::cout << 'T';
      } else {
        std::cout << '.';
      }
    }
    std::cout << '\n';
  }
  std::cout << "max height: " << *top_it << std::endl;
}

int get_minimum_x_velocity(const Target &t) {
  // brute force this because I don't feel like doing actual math
  int v = t.left_top.x;

  constexpr auto reaches = [](int v, int d) -> bool {
    int covered = 0;
    while (covered < d && v > 0) {
      covered += v;
      v--;
    }
    return (covered >= d);
  };

  do {
    v--;
  } while (reaches(v, t.left_top.x));

  return (v + 1);
}

int get_maximum_y_velocity(const Target &t) { return -(t.right_bottom.y) - 1; }

int main() {
  const auto targets = get_game_input_from_file("input.txt");

  std::cout << targets << std::endl;

  for (const auto &target : targets) {
    const Velocity velocity{get_minimum_x_velocity(target), get_maximum_y_velocity(target)};

    const auto trajectory = Probe::launch(velocity).shoot_for_target(target);

    std::cout << "velocity: " << velocity << std::endl;
    print_trajectory_and_target(trajectory, target);
  }
}
