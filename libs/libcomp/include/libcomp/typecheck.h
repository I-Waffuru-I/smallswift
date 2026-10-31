#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
// #include "libcomp.h"
namespace typecheck {

typedef std::string str;

struct Type {
   enum class Kind { Int, Bool, Void, Func, Error, Any };

   Kind kind;
   std::vector<Type> params;
   std::shared_ptr<Type> ret;

   explicit Type(Kind k = Kind::Error) : kind(k) {}
   static Type Int() { return Type(Kind::Int); }
   static Type Any() { return Type(Kind::Any); }
   static Type Bool() { return Type(Kind::Bool); }
   static Type Void() { return Type(Kind::Void); }
   static Type Error() { return Type(Kind::Error); }
   static Type Func(std::vector<Type> params, Type ret);

   bool operator==(const Type& o) const;
   bool operator!=(const Type& o) const { return !(*this == o); }
   str toString() const;

};

struct Symbol {
   Type type;
   bool isMutable;
};

struct Scope {
   std::unordered_map<str, Symbol> symbols_;
   Scope *parent_;

   Scope(Scope *parent = nullptr) : parent_(parent) {}

   // true/false volgens of symbol al bestaat
   bool declare(str &name, Symbol sym);
   const Symbol *lookup(str &name);
};

void registerStdFuncs(Scope& s);


} // namespace typecheck
