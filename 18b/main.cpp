#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <optional>
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

struct Number;
struct Regular;
struct Pair;

class Number {
private:
  std::optional<Number *> _parent{};

public:
  Number() = default;
  explicit Number(std::optional<Number *> p) : _parent(p) {}
  virtual ~Number() {}
  virtual std::ostream &          out(std::ostream &os) const                             = 0;
  virtual bool                    contains(const Number *n) const                         = 0;
  virtual std::optional<Number *> find_if(std::function<bool(const Number *)> predicate)  = 0;
  virtual std::optional<Number *> rfind_if(std::function<bool(const Number *)> predicate) = 0;

  virtual std::optional<Number *>
  find_left_of_if(const Number *n, std::function<bool(const Number *)> predicate) = 0;

  virtual std::optional<Number *>
  find_right_of_if(const Number *n, std::function<bool(const Number *)> predicate) = 0;

  virtual const Number *leftmost() const  = 0;
  virtual Number *      leftmost()        = 0;
  virtual const Number *rightmost() const = 0;
  virtual Number *      rightmost()       = 0;

  virtual std::unique_ptr<Number> clone(std::optional<Number *> new_parent = {}) const = 0;

  virtual size_t magnitude() const = 0;

  virtual std::optional<Number *>       parent() { return _parent; }
  virtual std::optional<const Number *> parent() const { return _parent; }

  virtual const Number *root() const {
    if (parent().has_value()) return parent().value()->root();
    return this;
  }
  virtual Number *root() {
    if (parent().has_value()) return parent().value()->root();
    return this;
  }

  virtual size_t depth() const {
    const auto p = parent();
    if (p.has_value()) return 1 + p.value()->depth();
    return 0;
  }
};

std::ostream &operator<<(std::ostream &os, const Number *n) { return n->out(os); }
std::ostream &operator<<(std::ostream &os, const std::unique_ptr<Number> &n) { return n->out(os); }

struct Regular final : public Number {
  unsigned value{};

  Regular() = default;
  explicit Regular(std::optional<Number *> p, unsigned v) : Number(p), value(v) {}

  bool contains(const Number *n) const override { return false; }

  std::optional<Number *> find_if(std::function<bool(const Number *)> predicate) override {
    if (predicate(this)) return {this};
    return {};
  }

  std::optional<Number *> rfind_if(std::function<bool(const Number *)> predicate) override {
    if (predicate(this)) return {this};
    return {};
  }

  std::optional<Number *> find_left_of_if(const Number *                      n,
                                          std::function<bool(const Number *)> predicate) override {
    return {};
  }

  std::optional<Number *> find_right_of_if(const Number *                      n,
                                           std::function<bool(const Number *)> predicate) override {
    return {};
  }

  const Number *leftmost() const override { return this; }
  Number *      leftmost() override { return this; }
  const Number *rightmost() const override { return this; }
  Number *      rightmost() override { return this; }

  std::unique_ptr<Number> clone(std::optional<Number *> new_parent = {}) const override {
    return std::make_unique<Regular>(new_parent, value);
  }

  size_t magnitude() const override { return value; }

  std::ostream &out(std::ostream &os) const override {
    os << value;
    return os;
  }

  virtual ~Regular() {}
};

struct Pair final : public Number {
  std::unique_ptr<Number> x = std::make_unique<Regular>(this, 0);
  std::unique_ptr<Number> y = std::make_unique<Regular>(this, 0);

  Pair() = default;
  explicit Pair(std::optional<Number *> p) : Number(p) {}

  bool contains(const Number *n) const override {
    return (n == this) || x->contains(n) || y->contains(n);
  }

  const Number *leftmost() const override { return x->leftmost(); }
  Number *      leftmost() override { return x->leftmost(); }
  const Number *rightmost() const override { return y->rightmost(); }
  Number *      rightmost() override { return y->rightmost(); }

  std::optional<Number *> find_if(std::function<bool(const Number *)> predicate) override {
    if (predicate(this)) return {this};

    const auto maybe_x = x->find_if(predicate);
    if (maybe_x.has_value()) return maybe_x;
    return y->find_if(predicate);
  }

  std::optional<Number *> rfind_if(std::function<bool(const Number *)> predicate) override {
    if (predicate(this)) return {this};

    const auto maybe_y = y->rfind_if(predicate);
    if (maybe_y.has_value()) return maybe_y;
    return x->rfind_if(predicate);
  }

  std::optional<Number *> find_left_of_if(const Number *                      n,
                                          std::function<bool(const Number *)> predicate) override {
    if (n == y.get() || n->leftmost() == y->leftmost()) return x->rfind_if(predicate);
    if (n == x.get() || n->leftmost() == x->leftmost()) return {};
    if (x->contains(n)) return x->find_left_of_if(n, predicate);
    if (y->contains(n)) return y->find_left_of_if(n, predicate);
    return {};
  }

  std::optional<Number *> find_right_of_if(const Number *                      n,
                                           std::function<bool(const Number *)> predicate) override {
    if (n == x.get() || n->rightmost() == x->rightmost()) return y->find_if(predicate);
    if (n == y.get() || n->rightmost() == y->rightmost()) return {};
    if (y->contains(n)) return y->find_right_of_if(n, predicate);
    if (x->contains(n)) return x->find_right_of_if(n, predicate);
    return {};
  }

  std::unique_ptr<Number> clone(std::optional<Number *> new_parent = {}) const override {
    auto out = std::make_unique<Pair>(new_parent);

    auto new_x = x->clone(out.get());
    auto new_y = y->clone(out.get());

    out->x.swap(new_x);
    out->y.swap(new_y);

    return out;
  }

  size_t magnitude() const override { return (x->magnitude() * 3) + (y->magnitude() * 2); }

  std::ostream &out(std::ostream &os) const override {
    os << '[' << x << ',' << y << ']';
    return os;
  }

  virtual ~Pair() {}
};

size_t find_top_level_comma(std::string_view str) {
  size_t depth = 0;
  size_t i     = 0;
  while (i < str.size()) {
    if (str[i] == ',' && depth == 1) return i;
    if (str[i] == '[') {
      depth++;
    } else if (str[i] == ']') {
      depth--;
    }
    i++;
  }
  throw std::runtime_error("no top-level comma found");
}

// forward declarations to alllow recursion
std::unique_ptr<Number> parse_regular_number(std::string_view        str,
                                             std::optional<Number *> parent = {});
std::unique_ptr<Number> parse_pair(std::string_view str, std::optional<Number *> parent = {});

std::unique_ptr<Number> parse_number(std::string_view str, std::optional<Number *> parent = {}) {
  if (str[0] == '[') return parse_pair(str, parent);
  return parse_regular_number(str, parent);
}

std::unique_ptr<Number> parse_regular_number(std::string_view str, std::optional<Number *> parent) {
  return std::make_unique<Regular>(parent, std::atoi(str.data()));
}

std::unique_ptr<Number> parse_pair(std::string_view str, std::optional<Number *> parent) {
  auto out = std::make_unique<Pair>(parent);

  const auto comma_idx = find_top_level_comma(str);
  const auto x_size    = comma_idx - 1;
  const auto y_size    = str.size() - 1 - comma_idx;

  out->x = parse_number(str.substr(1, x_size), out.get());
  out->y = parse_number(str.substr(comma_idx + 1, y_size), out.get());

  return out;
}

auto get_game_input_from_file(std::string_view filename) {
  const auto lines = get_non_empty_lines_from_file(filename);

  std::vector<std::unique_ptr<Number>> numbers{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(numbers), [](auto line) {
    return parse_number(line);
  });

  return numbers;
}

bool is_regular(const Number *n) {
  if (dynamic_cast<const Regular *>(n)) return true;
  return false;
}

bool is_pair(const Number *n) {
  if (dynamic_cast<const Pair *>(n)) return true;
  return false;
}

void explode(Pair *pair) {
  Number *root = pair->root();

  auto maybe_left_regular  = root->find_left_of_if(pair, is_regular);
  auto maybe_right_regular = root->find_right_of_if(pair, is_regular);

  auto *left_regular  = dynamic_cast<Regular *>(maybe_left_regular.value_or(nullptr));
  auto *right_regular = dynamic_cast<Regular *>(maybe_right_regular.value_or(nullptr));
  auto *left_child    = dynamic_cast<Regular *>(pair->x.get());
  auto *right_child   = dynamic_cast<Regular *>(pair->y.get());

  if (!left_child || !right_child) { throw std::runtime_error("reeeeemmmmmmiiiiiiiiii"); }

  if (left_regular) { left_regular->value += left_child->value; }
  if (right_regular) { right_regular->value += right_child->value; }

  Pair *parent = dynamic_cast<Pair *>(pair->parent().value_or(nullptr));

  if (!parent) { throw std::runtime_error("maaaamaaaaaaa"); }

  if (parent->x.get() == pair) {
    parent->x.reset();
    parent->x = std::make_unique<Regular>(parent, 0);
  }
  if (parent->y.get() == pair) {
    parent->y.reset();
    parent->y = std::make_unique<Regular>(parent, 0);
  }
}

void split(Regular *regular) {
  const auto val       = regular->value;
  const auto new_x_val = (val / 2);
  const auto new_y_val = ((val + 1) / 2);

  Pair *parent = dynamic_cast<Pair *>(regular->parent().value_or(nullptr));
  if (!parent) { throw std::runtime_error("maaaamaaaaaaa"); }

  std::unique_ptr<Number> new_pair   = std::make_unique<Pair>(parent);
  auto *                  new_pair_p = dynamic_cast<Pair *>(new_pair.get());

  dynamic_cast<Regular *>(new_pair_p->x.get())->value = new_x_val;
  dynamic_cast<Regular *>(new_pair_p->y.get())->value = new_y_val;

  if (parent->x.get() == regular) parent->x.swap(new_pair);
  if (parent->y.get() == regular) parent->y.swap(new_pair);

  // note the unique_ptr on the stack will clean up the old number
}

void reduce(Number *number) {
  bool is_stable = false;
  while (!is_stable) {
    is_stable = true; // assert stability, we'll set it to false if something changes

    // see if anything needs to explode
    auto to_explode =
        number->find_if([&](const Number *n) { return is_pair(n) && (n->depth() >= 4); });
    if (to_explode.has_value()) {
      explode(dynamic_cast<Pair *>(to_explode.value()));
      is_stable = false;
      continue;
    }

    // see if anything needs to split
    auto to_split = number->find_if([](const Number *n) {
      const auto *rn = dynamic_cast<const Regular *>(n);
      if (rn != nullptr) return rn->value >= 10;
      return false;
    });
    if (to_split.has_value()) {
      split(dynamic_cast<Regular *>(to_split.value()));
      is_stable = false;
    }
  }
}

std::unique_ptr<Number> add(const Number *lhs, const Number *rhs) {
  auto result = std::make_unique<Pair>();
  auto x      = lhs->clone(result.get());
  auto y      = rhs->clone(result.get());
  result->x.swap(x);
  result->y.swap(y);

  reduce(result.get());

  return result;
}

int main() {
  const auto numbers = get_game_input_from_file("input.txt");

  size_t max = 0;
  for (const auto &n : numbers) {
    for (const auto &m : numbers) {
      if (n != m) max = std::max(max, add(n.get(), m.get())->magnitude());
    }
  }

  std::cout << max << std::endl;
}
