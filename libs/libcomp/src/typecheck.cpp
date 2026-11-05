#include "libcomp/typecheck.h"
#include <format>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace typecheck;

bool Scope::declare(str &name, Symbol sym) {
  // returns een pair<iter, bool>
  return symbols_.emplace(name, std::move(sym)).second;
};
const Symbol *Scope::lookup(str &name) {
  for (const Scope *s = this; s; s = s->parent_) {
    auto it = s->symbols_.find(name);
    if (it != s->symbols_.end())
      return &it->second;
  }
  return nullptr;
};

void Scope::error(str out) {
  std::cerr << "line [" << dbg.linenr << "]" << out << std::endl;
  exit(1);
}
