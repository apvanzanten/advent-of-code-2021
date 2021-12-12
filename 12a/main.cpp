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

struct Cave {
  std::string         name{};
  bool                is_big = false;
  std::vector<size_t> connected_caves{};

  bool operator==(const Cave &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Cave &c) {
    os << c.name << " -> { ";
    for (size_t c : c.connected_caves) {
      os << c << " ";
    }
    os << "}";
    return os;
  }
};

constexpr bool is_upper_case(char c) { return (c >= 'A' && c <= 'Z'); }

std::tuple<std::string_view, std::string_view> split_line(std::string_view line) {
  return std::make_tuple(line.substr(0, line.find('-')), line.substr(line.find('-') + 1));
}

size_t insert_or_find(std::vector<Cave> &caves, std::string_view name) {
  const auto it = std::find_if(
      caves.begin(), caves.end(), [name](const auto &cave) { return cave.name == name; });

  if (it == caves.end()) {
    caves.emplace_back(std::string{name}, is_upper_case(name[0]));
    return caves.size() - 1;
  }
  return std::distance(caves.begin(), it);
}

auto get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  std::vector<Cave> caves{};

  for (const auto &line : lines) {
    const auto [lhs_name, rhs_name] = split_line(line);

    const auto lhs_idx = insert_or_find(caves, lhs_name);
    const auto rhs_idx = insert_or_find(caves, rhs_name);

    caves[lhs_idx].connected_caves.push_back(rhs_idx);
    caves[rhs_idx].connected_caves.push_back(lhs_idx);

    std::sort(caves[lhs_idx].connected_caves.begin(), caves[lhs_idx].connected_caves.end());
    std::sort(caves[rhs_idx].connected_caves.begin(), caves[rhs_idx].connected_caves.end());
  }

  return caves;
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

using CaveIdx = size_t; // index in caves vector
using NodeIdx = size_t; // index in nodes vector

struct Node {
  CaveIdx              cave_idx{};
  std::vector<NodeIdx> children{};
  NodeIdx              parent{};
};

struct PathTree {
  // NOTE we don't use iterators so we don't have to worry about invalidation
  std::vector<Node> nodes{};

  PathTree(CaveIdx start_idx) : nodes({{start_idx, {}}}) {}

  Node &      root() { return nodes.front(); }
  const Node &root() const { return nodes.front(); }

  bool operator==(const PathTree &) const = default;
};

void print_node_for_caves(const Node &             node,
                          const PathTree &         tree,
                          const std::vector<Cave> &caves,
                          int                      depth = 0) {
  for (int i = 0; i < depth; i++) {
    std::cout << "--";
  }
  std::cout << caves[node.cave_idx].name;
  if (!node.children.empty()) {
    std::cout << "{" << std::endl;
    for (size_t child_idx : node.children) {
      print_node_for_caves(tree.nodes[child_idx], tree, caves, depth + 1);
    }
    for (int i = 0; i < depth; i++) {
      std::cout << "--";
    }
    std::cout << "}";
  }
  std::cout << std::endl;
}

void print_tree_for_caves(const PathTree &tree, const std::vector<Cave> &caves) {
  print_node_for_caves(tree.root(), tree, caves);
}

bool is_cave_in_path(const PathTree &tree, NodeIdx end_of_path, CaveIdx cave_idx) {
  NodeIdx current_node = end_of_path;
  while (true) {
    if (tree.nodes[current_node].cave_idx == cave_idx) return true;
    if (current_node == 0) break;
    current_node = tree.nodes[current_node].parent;
  };
  return false;
}

bool try_direction(PathTree &               tree,
                   const std::vector<Cave> &caves,
                   NodeIdx                  current_idx,
                   CaveIdx                  target_idx) {
  const Cave &target = caves[target_idx];

  // check that this is a new direction
  for (NodeIdx n : tree.nodes[current_idx].children) {
    if (tree.nodes[n].cave_idx == target_idx) return false;
  }

  // if target is end then we are good
  if (target.name == "end") {
    tree.nodes.emplace_back(target_idx, std::vector<NodeIdx>{}, current_idx);
    tree.nodes[current_idx].children.push_back(tree.nodes.size() - 1);
    return true;
  }

  // check that this cave is either big, or has not yet been taken
  if (!target.is_big && is_cave_in_path(tree, current_idx, target_idx)) { return false; }

  // add node to children, and recurse
  tree.nodes.emplace_back(target_idx, std::vector<NodeIdx>{}, current_idx);
  NodeIdx new_node = tree.nodes.size() - 1;

  tree.nodes[current_idx].children.push_back(new_node);

  bool path_reached_end = false;
  for (CaveIdx dir : target.connected_caves) {
    const bool dir_reaches_end = try_direction(tree, caves, new_node, dir);
    if (dir_reaches_end) path_reached_end = true;
  }

  if (!path_reached_end) {
    // no path coming through here reached the end, thus we need to remove the new node from the
    // tree
    tree.nodes.pop_back();
    tree.nodes[current_idx].children.pop_back();

    return false;
  }

  return true;
}

int main() {
  const auto caves = get_game_input_from_file("input.txt");

  const CaveIdx start_idx =
      std::distance(caves.begin(), std::find_if(caves.begin(), caves.end(), [](const auto &cave) {
                      return cave.name == "start";
                    }));
  const CaveIdx end_idx =
      std::distance(caves.begin(), std::find_if(caves.begin(), caves.end(), [](const auto &cave) {
                      return cave.name == "end";
                    }));

  PathTree tree(start_idx);

  std::for_each(caves[start_idx].connected_caves.begin(),
                caves[start_idx].connected_caves.end(),
                [&](CaveIdx dir) { try_direction(tree, caves, 0, dir); });

  // print_tree_for_caves(tree, caves);

  // count number of times we reached the end, that is the number of valid paths
  const auto solution = std::count_if(
      tree.nodes.begin(), tree.nodes.end(), [end_idx](const Node &n) { return (n.cave_idx == end_idx); });
  
  std::cout << solution << std::endl;
}