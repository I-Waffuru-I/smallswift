#pragma once
#include "libcomp/typecheck.h"
namespace libcomp {

using namespace typecheck;

struct Stm_ {
  virtual ~Stm_() {};
  virtual void debug() = 0;
  virtual Type check(Scope &s) = 0;
};
struct Exp_ {
  virtual ~Exp_() {}
  virtual void debug() = 0;
  virtual Type check(Scope &s) = 0;
};
struct ExpList_ {
  virtual ~ExpList_() {};
  virtual void debug() = 0;
  virtual Type check(Scope &s) = 0;
};

typedef Stm_ *Stm;
typedef Exp_ *Exp;
typedef ExpList_ *Expl;

struct NumExp : public Exp_ {
  int num;
  NumExp(int n);
  void debug() override;
  Type check(Scope &s) override;
};

struct TFExp : public Exp_ {
  bool val;
  TFExp(bool value);
  void debug() override;
  Type check(Scope &s) override;
};

enum OpType { Plus, Minus, Multiply, Divide };
enum BoolExpType { And, Or, Eq, Neq, Lt, Gt, Le, Ge };
struct BoolExp : public Exp_ {
  Exp l, r;
  BoolExpType t;
  BoolExp(Exp left, BoolExpType type, Exp right);
  void debug() override;
  Type check(Scope &s) override;
};
struct OpExp : public Exp_ {
  Exp l;
  OpType op;
  Exp r;
  OpExp(Exp l, OpType op, Exp r);
  void debug() override;
  Type check(Scope &s) override;
};

struct ParenExp : public Exp_ {
  Exp exp;
  ParenExp(Exp expr);
  void debug() override;
  Type check(Scope &s) override;
};

struct LineExp : public ExpList_ {
  Exp exp;
  Expl tail;
  LineExp(Exp e, Expl prev);
  void debug() override;
  Type check(Scope &s) override;
};
struct EndExp : public ExpList_ {
  Exp exp;
  EndExp(Exp exp);
  void debug() override;
  Type check(Scope &s) override;
};

} // namespace libcomp
