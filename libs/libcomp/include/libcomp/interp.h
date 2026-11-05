#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace interp {

typedef std::string str;

struct Callable;
class Context;

struct Value {
  std::variant<std::monostate, int, bool, std::shared_ptr<Callable>> val;

  static Value Void() { return {std::monostate{}}; }
  static Value Int(int i) { return {i}; }
  static Value Bool(bool b) { return {b}; }
  static Value Fn(std::shared_ptr<Callable> fn) { return {std::move(fn)}; }

  // als dit ooit bad_variant_access throwed, is er een typecheck fout
  int asInt() { return std::get<int>(val); }
  bool asBool() { return std::get<bool>(val); }
  Callable &asFn() const;
};

// make callable of each func declare right before compile
struct Callable {
  virtual ~Callable() {};
  virtual Value call(std::unordered_map<str, Value> &args, Context &c) {
    return Value::Void();
  };
};

inline Callable &Value::asFn() const {
  return *std::get<std::shared_ptr<Callable>>(val);
};

// thrown by return statement
struct ReturnState {
  bool isset;
  Value val;
  ReturnState() : isset(false), val(Value::Void()) {};
};

// scope, but for interp
class Context : public std::enable_shared_from_this<Context> {

private:
  std::unordered_map<str, Value> values_;
  std::shared_ptr<Context> parent_;

public:
  std::shared_ptr<ReturnState> ret;

private:
  Context *findContext(str id);

public:
  Context(std::shared_ptr<Context> parent = nullptr)
      : parent_(parent), ret(parent ? parent->ret : nullptr) {}

  void declare(str id, Value val);
  void assign(str id, Value val);
  Value *lookup(str &name);
  void error(str out);
  void info(str out);
};

} // namespace interp
