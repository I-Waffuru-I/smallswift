#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "libcomp/util.h"
#include "libcomp/interp.h"
#include <variant>
#include <vector>
// #include "libcomp.h"
namespace typecheck {

using namespace util;


struct Symbol {
   Type type;
   bool isMutable;
};

struct DebugInfo {
   size_t scopedepth;
   size_t linenr;
};

struct Scope {
   std::unordered_map<str, Symbol> symbols_;
   Scope *parent_;
   DebugInfo dbg;

   Scope(Scope *parent = nullptr) : parent_(parent) {
      if(parent) dbg = parent->dbg;
   }

   // true/false volgens of symbol al bestaat
   bool declare(str &name, Symbol sym);
   const Symbol *lookup(str &name);
   void error(str out);
};



} // namespace typecheck
