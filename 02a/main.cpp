#include <numeric>
#include <algorithm>
#include <vector>
#include <string_view>
#include <iostream>
#include <cstdlib>

constexpr auto data = std::array<std::string_view, 1000>{
    "forward 4", "down 8", "down 8", "up 2", "up 7", "forward 5", "forward 5", "up 7", "down 6", "down 3", "down 1", "forward 5", "forward 9", "up 2", "down 9", "forward 4", "up 5", "forward 7", "down 2", "forward 7", "down 2", "forward 4", "up 3", "down 9", "up 8", "down 2", "down 6", "up 1", "forward 3", "down 6", "down 2", "forward 9", "up 1", "forward 5", "down 1", "forward 2", "up 2", "forward 4", "down 3", "down 8", "up 2", "down 3", "up 4", "down 8", "forward 7", "forward 9", "down 7", "down 1", "forward 5", "up 3", "down 6", "down 6", "forward 1", "down 9", "forward 6", "forward 9", "forward 2", "forward 5", "forward 7", "down 1", "up 6", "up 7", "forward 8", "forward 6", "forward 2", "down 5", "up 3", "up 4", "down 9", "up 4", "down 9", "up 4", "down 5", "forward 3", "down 8", "up 2", "down 2", "forward 7", "down 7", "forward 6", "down 2", "forward 5", "down 1", "forward 9", "down 9", "down 5", "forward 2", "forward 3", "forward 6", "forward 1", "down 8", "forward 2", "forward 1", "forward 9", "down 8", "forward 8", "up 1", "up 2", "forward 2", "forward 7", "down 2", "up 9", "forward 5", "forward 5", "up 5", "down 1", "up 8", "forward 3", "up 5", "forward 2", "up 8", "up 7", "forward 4", "down 6", "up 1", "up 6", "forward 5", "down 8", "forward 4", "down 7", "forward 5", "down 4", "down 9", "forward 2", "down 5", "down 2", "down 3", "forward 8", "down 8", "down 2", "down 5", "down 6", "up 8", "down 1", "up 7", "up 4", "up 1", "up 6", "forward 6", "forward 6", "forward 8", "up 5", "forward 4", "forward 5", "forward 3", "down 8", "forward 9", "forward 6", "forward 6", "up 1", "up 8", "forward 2", "up 9", "down 1", "up 7", "up 3", "down 3", "forward 2", "down 5", "up 8", "forward 3", "up 5", "down 3", "down 3", "up 7", "forward 2", "forward 3", "forward 6", "forward 9", "up 3", "forward 1", "up 9", "down 8", "forward 5", "down 8", "forward 9", "down 1", "forward 7", "forward 9", "forward 2", "down 6", "up 6", "down 2", "down 1", "forward 7", "down 3", "forward 3", "down 3", "forward 1", "forward 6", "forward 1", "down 4", "down 4", "down 5", "forward 3", "forward 1", "up 8", "forward 7", "down 6", "up 6", "down 5", "up 6", "down 3", "down 8", "down 9", "forward 2", "up 8", "forward 1", "forward 2", "forward 7", "forward 5", "up 6", "down 9", "up 9", "forward 7", "forward 6", "forward 7", "down 8", "down 6", "forward 5", "down 2", "down 5", "down 3", "down 4", "up 5", "down 5", "forward 7", "forward 2", "down 1", "forward 6", "up 8", "down 3", "down 5", "down 3", "forward 3", "up 2", "forward 9", "forward 2", "up 4", "down 3", "down 7", "forward 9", "forward 6", "up 1", "up 2", "down 5", "up 8", "forward 9", "forward 2", "down 3", "down 6", "up 3", "down 9", "down 2", "up 4", "down 3", "up 7", "forward 3", "up 9", "down 3", "down 9", "down 1", "down 1", "forward 7", "down 9", "forward 3", "up 6", "down 8", "down 3", "forward 7", "forward 1", "up 4", "forward 8", "forward 1", "forward 9", "up 9", "forward 4", "up 2", "down 6", "down 5", "down 8", "down 2", "down 4", "forward 5", "down 8", "down 1", "forward 5", "forward 9", "down 4", "forward 5", "forward 4", "forward 4", "up 6", "down 7", "down 2", "forward 8", "down 7", "forward 7", "forward 7", "forward 3", "down 3", "forward 6", "down 5", "down 5", "forward 3", "down 7", "up 3", "up 6", "forward 8", "down 3", "down 6", "forward 5", "forward 4", "down 4", "down 3", "down 1", "down 4", "down 2", "forward 1", "forward 5", "down 9", "forward 8", "down 7", "forward 4", "down 5", "down 5", "forward 7", "forward 9", "down 5", "down 8", "up 9", "forward 1", "down 9", "up 1", "down 8", "forward 4", "up 8", "up 7", "down 4", "forward 2", "forward 9", "up 9", "forward 4", "forward 5", "forward 5", "forward 4", "forward 4", "down 8", "forward 3", "forward 3", "forward 1", "forward 7", "forward 7", "up 2", "forward 9", "down 8", "forward 3", "down 3", "down 3", "down 4", "forward 9", "forward 9", "forward 7", "forward 9", "down 6", "forward 6", "down 4", "forward 7", "down 3", "forward 2", "down 9", "down 9", "up 2", "down 7", "down 6", "up 5", "forward 6", "forward 5", "down 9", "forward 8", "down 9", "forward 9", "down 7", "up 8", "forward 5", "forward 1", "down 5", "forward 1", "down 4", "up 6", "up 1", "down 5", "forward 3", "down 1", "up 7", "down 8", "up 5", "down 8", "up 6", "forward 6", "down 8", "up 2", "forward 5", "down 5", "down 7", "down 7", "forward 8", "forward 6", "forward 2", "forward 3", "forward 3", "forward 9", "down 7", "up 8", "up 1", "forward 8", "down 5", "down 7", "forward 2", "down 9", "down 5", "down 5", "forward 6", "forward 1", "forward 8", "down 3", "down 3", "down 7", "up 3", "down 3", "down 5", "down 1", "forward 3", "forward 2", "forward 4", "forward 1", "forward 3", "forward 6", "down 6", "down 4", "forward 2", "down 8", "up 1", "down 7", "down 6", "down 3", "down 6", "forward 8", "up 7", "down 7", "up 7", "down 1", "forward 2", "forward 9", "up 8", "down 2", "down 3", "down 7", "down 2", "up 2", "down 1", "down 7", "up 6", "down 4", "forward 9", "down 8", "down 1", "forward 5", "forward 1", "up 7", "up 9", "up 9", "down 5", "down 7", "down 2", "down 6", "down 3", "forward 8", "forward 4", "up 3", "down 9", "up 3", "down 6", "up 8", "forward 7", "down 7", "up 5", "down 1", "down 3", "up 4", "forward 2", "down 7", "down 3", "down 7", "up 1", "forward 8", "down 3", "forward 7", "down 8", "forward 5", "forward 8", "down 8", "up 4", "up 8", "forward 3", "down 7", "up 6", "down 9", "forward 4", "forward 4", "forward 3", "up 4", "down 4", "down 7", "forward 6", "down 7", "down 8", "up 5", "down 4", "up 6", "up 6", "up 4", "down 7", "forward 7", "up 4", "down 2", "up 2", "forward 6", "down 5", "down 1", "forward 2", "up 1", "down 4", "up 2", "down 7", "down 5", "up 5", "forward 6", "up 2", "forward 2", "up 9", "up 4", "down 1", "down 3", "up 7", "up 5", "down 9", "down 2", "forward 9", "down 1", "up 9", "down 4", "down 8", "forward 3", "forward 1", "forward 4", "forward 9", "down 5", "down 5", "down 8", "up 4", "up 1", "down 9", "up 4", "forward 9", "up 1", "forward 7", "down 4", "up 2", "down 1", "forward 9", "down 9", "down 2", "forward 8", "up 2", "forward 6", "down 1", "up 9", "down 3", "down 2", "down 8", "down 2", "forward 8", "forward 2", "forward 8", "down 3", "up 6", "forward 5", "forward 4", "forward 7", "forward 1", "down 8", "forward 7", "down 9", "up 7", "up 5", "forward 1", "down 6", "down 6", "up 9", "up 9", "up 1", "forward 1", "forward 5", "up 1", "forward 2", "down 8", "up 9", "forward 2", "forward 8", "down 2", "up 5", "up 9", "down 5", "forward 2", "forward 4", "forward 2", "up 7", "down 9", "forward 5", "down 1", "down 6", "up 1", "forward 8", "down 1", "down 7", "up 2", "forward 4", "down 2", "up 6", "forward 6", "forward 3", "down 3", "forward 2", "down 2", "up 9", "forward 2", "up 1", "down 9", "down 4", "up 8", "forward 3", "down 9", "down 9", "forward 9", "forward 8", "up 8", "down 8", "up 8", "forward 4", "down 9", "up 5", "forward 8", "up 6", "forward 7", "up 6", "down 2", "down 3", "forward 9", "forward 5", "down 6", "forward 9", "down 5", "down 9", "down 7", "down 9", "down 3", "forward 4", "forward 2", "down 2", "down 7", "down 7", "up 2", "up 3", "forward 6", "up 7", "forward 4", "down 3", "forward 2", "down 1", "down 8", "forward 5", "down 3", "up 9", "forward 2", "forward 7", "down 4", "forward 1", "forward 8", "forward 9", "forward 5", "down 4", "up 3", "up 9", "forward 6", "forward 4", "forward 9", "down 3", "forward 1", "forward 9", "down 9", "down 5", "forward 9", "forward 4", "down 3", "down 9", "down 5", "up 6", "up 5", "forward 5", "up 8", "down 3", "forward 7", "up 3", "forward 9", "down 8", "forward 2", "forward 1", "forward 9", "down 9", "forward 1", "down 6", "forward 7", "up 3", "forward 7", "up 3", "down 1", "forward 5", "forward 5", "up 3", "forward 2", "down 3", "forward 8", "up 9", "forward 7", "down 7", "forward 5", "up 4", "forward 8", "down 1", "up 4", "down 2", "forward 2", "down 5", "down 5", "up 2", "forward 1", "down 3", "down 8", "forward 6", "forward 6", "down 5", "up 4", "down 7", "down 9", "up 9", "forward 7", "forward 4", "down 7", "down 5", "down 2", "down 9", "down 6", "down 7", "up 6", "up 7", "up 6", "down 4", "forward 9", "down 8", "down 7", "down 8", "down 4", "forward 5", "forward 1", "up 5", "forward 5", "forward 4", "down 3", "forward 8", "down 7", "down 9", "up 1", "down 1", "up 8", "up 6", "down 9", "up 9", "down 9", "forward 7", "down 3", "forward 6", "down 6", "forward 6", "down 9", "down 7", "up 1", "down 2", "up 2", "down 3", "down 1", "up 4", "forward 3", "down 3", "up 8", "down 3", "forward 3", "forward 6", "forward 6", "forward 6", "forward 7", "up 2", "forward 6", "forward 1", "up 4", "up 7", "down 5", "down 9", "forward 6", "down 4", "forward 6", "down 7", "down 2", "up 9", "up 3", "forward 8", "forward 5", "down 1", "down 6", "down 7", "down 5", "up 3", "up 9", "forward 2", "forward 5", "down 3", "down 2", "up 2", "forward 6", "forward 3", "down 8", "forward 7", "up 6", "forward 4", "down 8", "forward 6", "down 7", "forward 9", "forward 6", "forward 2", "forward 4", "up 5", "up 1", "forward 3", "forward 2", "up 3", "down 4", "down 3", "down 1", "up 8", "forward 6", "down 4", "down 9", "down 3", "up 8", "down 5", "forward 2", "down 3", "up 7", "down 3", "up 1", "up 1", "up 2", "up 1", "forward 4", "forward 1", "forward 4", "forward 3", "forward 8", "down 8", "up 5", "down 4", "down 4", "down 6", "down 9", "down 7", "forward 5", "forward 3", "up 3", "forward 6", "forward 5", "forward 2", "forward 6", "up 4", "forward 2", "up 3", "down 2", "forward 3", "down 8", "forward 1", "forward 2", "down 3", "down 5", "forward 6", "forward 3", "forward 6", "up 3", "forward 5", "forward 3", "forward 5", "down 6", "down 4", "down 4", "forward 3", "forward 3", "up 6", "up 8", "forward 5", "forward 1", "down 3", "down 8", "down 9", "up 3", "down 7", "forward 4", "forward 2", "down 2", "up 6", "down 1", "down 8", "forward 3", "up 1", "down 7", "down 7", "down 5", "forward 3", "down 8", "forward 3", "down 7", "down 5", "up 2", "forward 9", "down 8", "down 5", "forward 3", "forward 2", "forward 7", "up 8", "down 2", "down 5", "down 8", "down 9", "down 9", "down 1", "up 4", "forward 5", "up 1", "up 4", "forward 1", "down 1", "down 7", "up 9", "up 7", "down 5", "down 9", "down 9", "down 8", "forward 7", "down 3", "up 4", "down 7", "down 8", "forward 7", "forward 4", "up 9", "down 2", "up 7", "forward 5", "down 3", "forward 3", "forward 5", "forward 5", "down 2", "down 2", "down 7", "up 8", "up 9", "down 1", "forward 9", "forward 3", "up 3", "forward 9", "up 2", "down 7", "down 3", "forward 4", "down 5", "down 3", "up 5", "forward 4"};

struct Command
{
  enum class Direction
  {
    forward,
    up,
    down
  } direction{};

  int distance = 0;

  constexpr Command() = default;

  explicit constexpr Command(std::string_view str)
  {
    const auto delim_index = str.find(' ');
    const auto dir_str = str.substr(0, delim_index);
    const auto dist_str = str.substr(delim_index + 1);

    distance = std::atoi(dist_str.data());

    if (dir_str == "forward")
    {
      direction = Direction::forward;
    }
    else if (dir_str == "up")
    {
      direction = Direction::up;
    }
    else
    { // assume valid command
      direction = Direction::down;
    }
  }

  constexpr auto get_horizontal_distance()
  {
    if (direction == Direction::forward)
      return distance;
    return 0;
  }

  constexpr auto get_depth_distance()
  {
    if (direction == Direction::up)
      return -distance;
    if (direction == Direction::down)
      return distance;
    return 0;
  }
};

struct Position
{
  int horizontal = 0;
  int depth = 0;
};

int main()
{
  std::array<Command, 1000> commands;

  std::transform(data.begin(), data.end(), commands.begin(), [](auto e)
                 { return Command{e}; });

  const Position final_position = std::accumulate(commands.begin(), commands.end(), Position{},
                                                  [](Position p, Command c)
                                                  {
                                                    return Position{p.horizontal + c.get_horizontal_distance(), p.depth + c.get_depth_distance()};
                                                  });

  const auto position_product = final_position.horizontal * final_position.depth;

  std::cout << position_product << std::endl;
}