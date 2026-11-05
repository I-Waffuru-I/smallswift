
#include "libcomp/interp.h"
#include <cassert>
#include <iostream>
using namespace interp;

// true/false volgens of symbol al bestaat
void Context::declare(str id, Value val) { values_[id] = val; };

Context *Context::findContext(str id) {
  for (Context *c = this; c; c = c->parent_.get()) {
    auto it = c->values_.find(id);
    if (it != c->values_.end())
      return c;
  }
  return nullptr;
}
void Context::assign(str id, Value val) {
  Context *c = findContext(id);
  if (!c) {
    error("Identifier [" + id +
          "] is not found in any scope during assignment");
  }
  c->values_[id] = val;
}

Value *Context::lookup(str &name) {
  for (Context *s = this; s; s = s->parent_.get()) {
    auto it = s->values_.find(name);
    if (it != s->values_.end())
      return &it->second;
  }
  error("Identifier '" + name + "' is not found in any scope");
  // already exited, ignore warning
  return nullptr;
};


void Context::error(str out) {
  std::cerr << "Runtime error: " << out << std::endl;
  exit(1);
};
void Context::info(str out) {
  std::cerr << "INFO: " << out << std::endl;
};
