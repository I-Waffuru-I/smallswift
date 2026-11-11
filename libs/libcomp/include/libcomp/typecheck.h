#pragma once
#include "libcomp/util.h"
#include <memory>
#include <string>
#include <unordered_map>
// #include "libcomp.h"
namespace typecheck {

using namespace util;

struct Symbol {
  Type type;
  bool isMutable;
};

struct ReturnInfo {
  Type typeExp;
  bool hasReturned;

  void startNewFlow();
};

struct DebugInfo {
  size_t scopedepth;
  size_t linenr;
};

struct Scope {
  std::unordered_map<str, Symbol> symbols_;
  Scope *parent_;
  DebugInfo dbg;
  // nullptr if not in a function
  ReturnInfo* ret;

  Scope(Scope *parent = nullptr, ReturnInfo* r = nullptr) : parent_(parent), ret(r) {
    if (parent) {
      dbg = parent->dbg;
      if(parent->ret)
        ret = parent->ret;
    }
  }

  // true/false volgens of symbol al bestaat
  bool declare(str &name, Symbol sym);
  const Symbol *lookup(str &name);
  Scope *findFnScope();
  void error(str out);
};

} // namespace typecheck
