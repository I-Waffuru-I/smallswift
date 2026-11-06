#include "libcomp/cstd.h"
#include "libcomp/interp.h"
#include "libcomp/libcomp.h"
#include "libcomp/typecheck.h"
#include "libcomp/util.h"
#include <functional>
#include <memory>
#include <variant>

using namespace cstd;
using namespace util;
using namespace interp;

Value printval(std::unordered_map<str, Value> &params, Context &c) {
  for (auto const &p : params) {
    if (p.first != "_") {
			c.error("extraneous argument label '"+p.first+":' in print");
    }
    std::visit(overloaded{[](std::monostate) { std::cout << "()"; },
                          [](int i) { std::cout << i; },
                          [](bool b) { std::cout << (b ? "true" : "false"); },
                          [](const auto &) { std::cout << "<function>"; }},
               p.second.val);
  }
	std::cout << std::endl;
  return Value::Void();
}
void addPrintFn(interp::Context &root) {
  std::function<Value(std::unordered_map<str, Value> &, Context &)> fn =
      printval;
  auto nfn = std::make_shared<libcomp::NativeFn>(fn);
  Value val = Value::Fn(nfn);
  root.declare("print", val);
}
void addPrintFn(typecheck::Scope &root) {
  std::vector<NameType> params{NameType("_", Kind::Any)};
  libcomp::Symbol s = {Type::Func(params, Type::Void()), false};
  str name = "print";
  root.declare(name, s);
}

void cstd::loadStd(typecheck::Scope &root) { addPrintFn(root); }

void cstd::loadStd(interp::Context &root) { addPrintFn(root); }
