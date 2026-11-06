#include "libcomp/libcomp.h"
#include "libcomp/typecheck.h"
#include "libcomp/util.h"
#include <format>
#include <iostream>
#include <memory>
#include <variant>

using namespace libcomp;
using namespace interp;
using namespace typecheck;

void indent(size_t depth) {
  for (size_t d = 0; d < depth; d++)
    std::cout << "   ";
}
void printval(Value &val) {
  std::visit(overloaded{[](std::monostate) { std::cout << "()"; },
                        [](int i) { std::cout << i; },
                        [](bool b) { std::cout << (b ? "true" : "false"); },
                        [](const auto &) { std::cout << "<function>"; }},
             val.val);
  std::cout << std::endl;
}

Type typeFromKind(NameType nt) {
  switch (nt.kind) {
    case Kind::Int:  { return Type::Int(); }
    case Kind::Bool:{ return Type::Bool();}
    case Kind::Void:{ return Type::Void();}
    default :{ return Type::Error();}
  }
}

Value Closure::call(std::unordered_map<str, Value> &args, Context &) {
  std::shared_ptr<Context> local = std::make_shared<Context>(env);
  local->info(std::format("called fn '{}'", decl->name));
  for (size_t i = 0; i < args.size(); i++) 
    local->declare(decl->params[i].name, args[decl->params[i].name]);

  decl->block->interp(*local);

  if (local->ret && !local->ret->isset)
    local->error(std::format("Function '{}' ended without returning a value",
                           decl->name));
  return local->ret ? local->ret->val : Value::Void();
}

/*
 * functions
 */
void FunDeclare::debug(size_t depth) {
  std::cout << name << "(" << std::endl;
  for (NameType t : params) {
    indent(depth + 1);
    std::cout << t.toString();
  }
  std::cout << name << ")" << std::endl;
  block->debug(depth);
}
void FunDeclare::check(Scope &s) {
  s.declare(name, {Type::Func(params, rtype), false});
  Scope news (s);
  for (NameType p : params) {
    Symbol s { typeFromKind(p), false};
    news.declare(p.name, s);
  }
  block->check(news);
}
void FunDeclare::interp(Context &c) {
  std::shared_ptr<Closure> closure =
      std::make_shared<Closure>(this, c.shared_from_this());
  c.declare(name, Value::Fn(closure));
}

void FunCall::debug(size_t depth) {
  std::cout << name << "(";
  for (FnCallParam p : params) {
    std::cout << p.name << " : ";
    p.val->debug(depth);
  }
  std::cout << ")";
};
Type FunCall::check(Scope &s) {
  const Symbol *fn = s.lookup(name);
  if (params.size() != fn->type.params.size())
    s.error(
        std::format("Function '{}' param count doesn't match signature", name));

  for (size_t i = 0; i < params.size(); i++) {
    Type t = params[i].val->check(s);
    NameType p = fn->type.params[i];
    if (t.kind != p.kind)
      s.error(std::format(
          "Param '{}' of function '{}' types don't match: '{}' and '{}'",
          p.name, name, p.toString(), t.toString()));
  }
  return Type::Void();
};
Value FunCall::interp(Context &c) {
  Callable &call = c.lookup(name)->asFn();
  std::unordered_map<str, Value> vals;
  for (const FnCallParam &p : params)
    vals[p.name] = p.val->interp(c);

  Value ret = call.call(vals, c);
  printval(ret);
  return ret;
}

void ReturnStm::debug(size_t depth) {
  std::cout << "return ";
  exp->debug(depth);
}
void ReturnStm::check(Scope &s) {}
void ReturnStm::interp(Context &c) {}

void PrintStm::debug(size_t depth) {
  std::cout << "print (";
  exp->debug(depth);
  std::cout << ")";
};
void PrintStm::check(Scope &s) { exp->check(s); };
void PrintStm::interp(Context &c) {
  std::visit(overloaded{[](std::monostate) { std::cout << "()"; },
                        [](int i) { std::cout << i; },
                        [](bool b) { std::cout << (b ? "true" : "false"); },
                        [](const auto &) { std::cout << "<function>"; }},
             exp->interp(c).val);
  std::cout << std::endl;
}
