#include "libcomp/libcomp.h"
#include "libcomp/interp.h"
#include "libcomp/typecheck.h"
#include <cstddef>
#include <execution>
#include <format>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>
namespace libcomp {

// util om te printen tijdens debug
void indent(size_t depth) {
  for (size_t d = 0; d < depth; d++)
    std::cout << "   ";
}

/*
 * high level crap
 */
void StmList::debug(size_t depth) {
  for (Stm s : stms) {
    indent(depth);
    s->debug(depth);
    std::cout << ";" << std::endl;
  }
};
void StmList::check(Scope &s) {
  for (Stm st : stms) {
    st->check(s);
    s.dbg.linenr++;
  }
};
void StmList::interp(Context &c) {
  for (Stm st : stms) {
    st->interp(c);
  }
};

void ExpStm::debug(size_t depth) {
  indent(depth);
  exp->debug(depth);
};
void ExpStm::check(Scope &s) { exp->check(s); };
void ExpStm::interp(Context &c) { exp->interp(c); }

/*
 * blocks
 */
void BlockStm::debug(size_t depth) {
  std::cout << "{" << std::endl;
  stms->debug(depth + 1);
  std::cout << "}";
};
void BlockStm::check(Scope &s) {
  Scope inner(s);
  stms->check(inner);
};
void BlockStm::interp(Context &c) {
  Context newc(c);
  stms->interp(newc);
}

void DoBlockStm::debug(size_t depth) {
  std::cout << "do ";
  block->debug(depth);
};
void DoBlockStm::check(Scope &s) { block->check(s); };
void DoBlockStm::interp(Context &c) { block->interp(c); }

void IfStm::debug(size_t depth) {
  std::cout << "if ";
  cond->debug(depth);
  then->debug(depth);
  if (els) {
    std::cout << " else ";
    els->debug(depth);
  }
};
void IfStm::check(Scope &s) {
  Type c = cond->check(s);
  if (c.kind != typecheck::Kind::Bool) {
    s.error(
        std::format("Expected bool type in condition, not [{}]", c.toString()));
    exit(1);
  }
  then->check(s);
  if (els)
    els->check(s);
};
void IfStm::interp(Context &c) {
  bool co = cond->interp(c).asBool();
  if (co) {
    then->interp(c);
  } else if (els) {
    els->interp(c);
  }
}

/*
 * functions
 */

void FunDeclare::debug(size_t depth) {
  std::cout << name << "(" << std::endl;
  for (NameType t : params) {
    std::cout << t.toString();
  }
  std::cout << name << ")" << std::endl;
  block->debug(depth);
}
void FunDeclare::check(Scope &s) {
  Type ftype = Type::Func(params, rtype);
  s.declare(name, {ftype, false});
}
void FunDeclare::interp(Context &c) {
  c.declare(name,
            Value::Fn(std::make_shared<Closure>(this, c.shared_from_this())));
}
void FunCall::debug(size_t depth) {

};
Type FunCall::check(Scope &s) {
  const Symbol *fn = s.lookup(name);
  if (params.size() != fn->type.params.size())
    s.error(
        std::format("Function '{}' param count doesn't match signature", name));

  for (size_t i = 0; i < params.size(); i++) {
    Type t = params[i]->check(s);
    NameType p = fn->type.params[i];
    if (t.kind != p.kind)
      s.error(std::format(
          "Param '{}' of function '{}' types don't match: '{}' and '{}'",
          p.name, name, p.toString(), t.toString()));
  }
  return Type::Void();
};
Value FunCall::interp(Context &c) {}

void PrintStm::debug(size_t depth) {
  std::cout << "print (";
  exp->debug(depth);
  std::cout << ")";
};

void ReturnStm::debug(size_t depth) {
  std::cout << "return";
  exp->debug(depth);
}
void ReturnStm::check(Scope &s) {}
void ReturnStm::interp(Context &c) {}

void PrintStm::check(Scope &s) { exp->check(s); };
void PrintStm::interp(Context &c) {
  std::visit(overloaded{[](std::monostate) { std::cout << "()"; },
                        [](int i) { std::cout << i; },
                        [](bool b) { std::cout << (b ? "true" : "false"); },
                        [](const auto &) { std::cout << "<function>"; }},
             exp->interp(c).val);
  std::cout << std::endl;
}

/*
 * variables
 */
void DeclareStm::debug(size_t depth) {
  if (mut) {
    std::cout << "var ";
  } else {
    std::cout << "let ";
  }
  std::cout << id << " = ";
  val->debug(depth);
};
void DeclareStm::check(Scope &s) {
  const Symbol *prev = s.lookup(id);
  if (prev) {
    s.error("Invalid redeclaration of [" + id + "]");
  }
  Type t = val->check(s);
  Symbol sym = {t, mut};
  s.declare(id, sym);
};
void DeclareStm::interp(Context &c) { c.declare(id, val->interp(c)); }

void AssignStm::debug(size_t depth) {
  std::cout << id << " = ";
  val->debug(depth);
};
void AssignStm::check(Scope &s) {
  Type t = val->check(s);
  const Symbol *sy = s.lookup(id);
  if (!sy) {
    s.error("Tried to set [" + id + "] but it is not declared");
  } else if (!sy->isMutable) {
    s.error("Tried to set [" + id + "] but it is not defined as mutable");
  } else if (sy->type != t) {
    s.error(std::format("Tried to set [{}], can't assign [{}] to a [{}]", id,
                        sy->type.toString(), t.toString()));
  }
};
void AssignStm::interp(Context &c) { c.assign(id, val->interp(c)); }

void VariableExp::debug(size_t _) { std::cout << id; };
Type VariableExp::check(Scope &s) {
  const Symbol *sy = s.lookup(id);
  if (!sy) {
    s.error(std::format("Cannot find variable [{}] in scope", id));
  }
  return sy->type;
};
Value VariableExp::interp(Context &c) { return c.lookup(id); }

/*
 * operations
 */
char debugOpExp(OpType op) {
  switch (op) {
  case Plus: {
    return '+';
  }
  case Minus: {
    return '-';
  }
  case Multiply: {
    return '*';
  }
  case Divide: {
    return '/';
  }
  }
  exit(1);
}
OpExp::OpExp(Exp l, OpType op, Exp r) : l(l), op(op), r(r) {};
void OpExp::debug(size_t depth) {
  l->debug(depth);
  std::cout << debugOpExp(op);
  r->debug(depth);
}
Type OpExp::check(Scope &s) {
  Type lt = l->check(s);
  Type rt = r->check(s);

  if (lt != rt) {
    s.error(std::format(
        "Can't apply operation [{}] to different types [{}] and [{}]",
        debugOpExp(op), lt.toString(), rt.toString()));
  } else if (lt.kind != typecheck::Kind::Int) {
    s.error(std::format("Can't apply operation [{}] to types [{}] and [{}]",
                        debugOpExp(op), lt.toString(), rt.toString()));
  }
  return lt;
};
Value OpExp::interp(Context &c) {
  int vl = l->interp(c).asInt();
  int vr = r->interp(c).asInt();
  switch (op) {
  case Plus: {
    return Value::Int(vl + vr);
  }
  case Minus: {
    return Value::Int(vl - vr);
  }
  case Multiply: {
    return Value::Int(vl * vr);
  }
  case Divide: {
    return Value::Int(vl / vr);
  }
  }
}

const char *debugBoolExpType(BoolExpType t) {
  switch (t) {
  case Eq: {
    return "==";
  }
  case Neq: {
    return "!=";
  }
  case Le: {
    return "<=";
  }
  case Lt: {
    return "<";
  }
  case Ge: {
    return ">=";
  }
  case Gt: {
    return ">";
  }
  case And: {
    return "&&";
  }
  case Or: {
    return "||";
  }
  }
  exit(1);
}
BoolExp::BoolExp(Exp left, BoolExpType type, Exp right)
    : l(left), r(right), t(type) {};
void BoolExp::debug(size_t depth) {
  l->debug(depth);
  std::cout << debugBoolExpType(t);
  r->debug(depth);
}
Type BoolExp::check(Scope &s) {
  Type lt = l->check(s);
  Type rt = r->check(s);

  if (lt != rt) {
    std::cerr << "Mismatched types" << std::endl;
    exit(1);
  } else if (lt.kind == typecheck::Kind::Bool && t != And && t != Or) {
    s.error(std::format("Can't apply operation [{}] to types [{}] and [{}]",
                        debugBoolExpType(t), lt.toString(), rt.toString()));
  }
  return Type::Bool();
};
Value BoolExp::interp(Context &c) {
  Value vl = l->interp(c);
  Value vr = r->interp(c);
  switch (t) {
  case Eq: {
    return Value::Bool(vl.asInt() < vr.asInt());
  }
  case Neq: {
    return Value::Bool(vl.asInt() != vr.asInt());
  }
  case Le: {
    return Value::Bool(vl.asInt() <= vr.asInt());
  }
  case Lt: {
    return Value::Bool(vl.asInt() < vr.asInt());
  }
  case Ge: {
    return Value::Bool(vl.asInt() >= vr.asInt());
  }
  case Gt: {
    return Value::Bool(vl.asInt() > vr.asInt());
  }
  case And: {
    return Value::Bool(vl.asBool() && vr.asBool());
  }
  case Or: {
    return Value::Bool(vl.asBool() || vr.asBool());
  }
  }
}

ParenExp::ParenExp(Exp expr) : exp(expr) {};
void ParenExp::debug(size_t depth) {
  std::cout << '(';
  exp->debug(depth);
  std::cout << ')';
}
Type ParenExp::check(Scope &s) { return exp->check(s); }
Value ParenExp::interp(Context &c) { return exp->interp(c); }

/*
 * Literals
 */
NumExp::NumExp(int n) : num(n) {};
void NumExp::debug(size_t _) { std::cout << num; }
Type NumExp::check(Scope &_) { return Type::Int(); }
Value NumExp::interp(Context &_) { return Value::Int(num); }

TFExp::TFExp(bool val) : val(val) {};
void TFExp::debug(size_t _) {
  if (val) {
    std::cout << "true";
  } else {
    std::cout << "false";
  }
};
Type TFExp::check(Scope &_) { return Type::Bool(); };
Value TFExp::interp(Context &_) { return Value::Bool(val); }

} // namespace libcomp
