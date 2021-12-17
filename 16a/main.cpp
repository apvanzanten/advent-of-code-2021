#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
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
#include <variant>
#include <vector>

using BitVec = std::vector<bool>;
using BitIt  = BitVec::const_iterator;

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[ ";
  for (const auto &e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const BitVec &v) {
  os << "0b";
  int i = 0;
  for (const auto &e : v) {
    if (i++ == 4) {
      i = 1;
      os << "'";
    }
    os << e;
  }
  os << ";";
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

auto get_game_input_from_file(std::string_view filename) {
  return get_non_empty_lines_from_file(filename);
}

BitVec bits_from_hex(std::string_view s) {
  BitVec bits{};

  constexpr auto hex_char_to_int = [](char c) -> uint8_t {
    if (c < 'A') return (c - '0');
    return (c - 'A') + 10;
  };

  for (char c : s) {
    const auto val = hex_char_to_int(c);
    for (int bit = 3; bit >= 0; bit--) {
      bits.emplace_back(val & (1 << bit));
    }
  }

  return bits;
}

enum class Type : unsigned { literal = 4 };
enum class LengthType : unsigned { bits, packets };

template <typename T>
concept Enum = std::is_enum<T>::value;

std::ostream &operator<<(std::ostream &os, Enum auto e) {
  os << static_cast<std::underlying_type<decltype(e)>::type>(e);
  return os;
}

struct Header {
  unsigned   version{};
  Type       type{};
  LengthType length_type{};
  unsigned   length{};

  constexpr bool operator==(const Header &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Header &h) {
    os << "Header{" << h.version << "," << h.type << "," << h.length_type << "," << h.length << "}";
    return os;
  }
};

struct Packet {
  Header              header{};
  std::vector<Packet> subpackets{};
  int64_t             value{};

  bool operator==(const Packet &) const = default;

  friend std::ostream &operator<<(std::ostream &os, const Packet &p) {
    switch (p.header.type) {
    case Type::literal: os << "Literal{" << p.header << "," << p.value << "}"; break;
    default: os << "Operator{" << p.header << "," << p.subpackets << "}"; break;
    }
    return os;
  }
};

unsigned to_uint(BitIt begin, BitIt end) {
  if (std::distance(begin, end) > 32) throw std::invalid_argument("begin and end too far apart");

  return std::accumulate(begin, end, unsigned{0}, [](unsigned out, bool b) {
    return (b ? ((out << 1) | 1) : (out << 1));
  });
}

unsigned read_uint(BitIt &it, unsigned n) {
  const auto out = to_uint(it, std::next(it, n));
  std::advance(it, n);
  return out;
}

Header read_header(BitIt &it) {
  constexpr auto version_size     = 3;
  constexpr auto type_size        = 3;
  constexpr auto length_type_size = 1;

  const auto version = read_uint(it, version_size);
  const auto type    = Type{read_uint(it, type_size)};

  if (type == Type::literal) { return {version, type}; }

  const auto length_type = LengthType{read_uint(it, length_type_size)};

  const auto length_size = (length_type == LengthType::bits ? 15 : 11);
  const auto length      = read_uint(it, length_size);

  return {version, type, length_type, length};
}

Packet read_packet(BitIt &it) {
  Packet packet{};
  packet.header = read_header(it);

  if (packet.header.type == Type::literal) {
    // TODO
    while (*it) {
      std::advance(it, 5);
    }
    std::advance(it, 5);
    return packet;
  }

  switch (packet.header.length_type) {
  case LengthType::bits: {
    const auto end = std::next(it, packet.header.length);
    while (it != end) {
      packet.subpackets.push_back(read_packet(it));
    }
    break;
  }
  case LengthType::packets:
    for (unsigned p = 0; p < packet.header.length; p++) {
      packet.subpackets.push_back(read_packet(it));
    }
    break;
  }

  return packet;
}

unsigned get_version_sum(const Packet &packet) {
  if (packet.header.type == Type::literal) return packet.header.version;
  return packet.header.version +
         std::accumulate(packet.subpackets.begin(),
                         packet.subpackets.end(),
                         unsigned{0},
                         [](auto s, const Packet &p) { return s + get_version_sum(p); });
}

int main() {
  const auto packets_in_hex = get_game_input_from_file("input.txt");
  for (const auto &hex : packets_in_hex) {
    const auto bits     = bits_from_hex(hex);
    auto       it       = bits.begin();
    const auto packet   = read_packet(it);
    const auto solution = get_version_sum(packet);
    std::cout << solution << std::endl;
  }
}
