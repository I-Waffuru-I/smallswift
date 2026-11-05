#pragma once
#include "libcomp/interp.h"
#include "libcomp/typecheck.h"
#include <functional>
#include <memory>

namespace libcomp {

using namespace typecheck;
using namespace interp;

struct Stm_ {
  virtual ~Stm_() {};
  virtual void debug(size_t depth) = 0;
  virtual void check(Scope &s) = 0;
  virtual void interpfuncs(Context &c) = 0;
  virtual void interp(Context &c) = 0;
};
struct Exp_ {
  virtual ~Exp_() {}
  virtual void debug(size_t depth) = 0;
  virtual Type check(Scope &s) = 0;
  virtual Value interp(Context &c) = 0;
};

typedef Stm_ *Stm;
typedef Exp_ *Exp;

class StmList : public Stm_ {
public:
  std::vector<Stm> stms;
  void add(Stm s) { stms.push_back(s); }
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};
struct ExpStm : public Stm_ {
  Exp exp;
  ExpStm(Exp e) : exp(e) {};
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};
struct BlockStm : public Stm_ {
  StmList *stms;
  Type rtype;
  BlockStm(StmList *s) : stms(s) { rtype = Type::Void(); };
  BlockStm(StmList *s, Type r) : stms(s), rtype(r) {};
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};

struct DoBlockStm : public Stm_ {
  Stm block;
  DoBlockStm(Stm s) : block(s) {};
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};
struct IfStm : public Stm_ {
  Exp cond;
  Stm then;
  Stm els;
  IfStm(Exp c, Stm s, Stm e = nullptr) : cond(c), then(s), els(e) {};
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};

struct FunDeclare : public Stm_ {
  str name;
  std::vector<NameType> params;
  Stm block;
  Type rtype;

  FunDeclare(str name, std::vector<NameType> params, Stm b, Type ret)
      : name(name), params(params), block(b), rtype(ret) {}

  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};
struct FunCall : public Exp_ {
  str name;
  std::vector<Exp> params;

  FunCall(str name, std::vector<Exp> params) : name(name), params(params) {}

  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};
struct ReturnStm : public Stm_ {
  Exp exp;
  ReturnStm(Exp e) : exp(e) {};
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};

struct PrintStm : public Stm_ {
  Exp exp;

  PrintStm(Exp e) : exp(e) {}

  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};

struct NativeFn : Callable {
  std::function<Value(std::vector<Value> &, Context &)> fn;

  NativeFn(std::function<Value(std::vector<Value> &, Context &)> f)
      : fn(std::move(f)) {}

  Value call(std::vector<Value> &args, Context &c) override {
    return fn(args, c);
  }
};
struct Closure : Callable{
   FunDeclare *decl;
	std::shared_ptr<Context> env;

  Closure(FunDeclare* fn, std::shared_ptr<Context> parent) :  decl(fn), env(parent) {}

  Value call(std::vector<Value> &args, Context &) override {
    for (size_t i = 0; i < args.size(); i++)
      env->declare(decl->params[i].name, args[i]);

    return Value::Void();
  }
};

struct DeclareStm : public Stm_ {
  str id;
  Exp val;
  bool mut;
  Type exptype;
  DeclareStm(str id, Exp v, bool m, Type *etype = nullptr)
      : id(id), val(v), mut(m) {
    if (etype) {
      exptype = *etype;
    } else
      exptype = Type::Void();
  };
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};
struct AssignStm : public Stm_ {
  str id;
  Exp val;
  AssignStm(str id, Exp v) : id(id), val(v) {};
  void debug(size_t depth) override;
  void check(Scope &s) override;
  void interp(Context &c) override;
};
struct VariableExp : public Exp_ {
  str id;
  VariableExp(str id) : id(id) {};
  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};

enum OpType { Plus, Minus, Multiply, Divide };
enum BoolExpType { And, Or, Eq, Neq, Lt, Gt, Le, Ge };
struct BoolExp : public Exp_ {
  Exp l, r;
  BoolExpType t;
  BoolExp(Exp left, BoolExpType type, Exp right);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};
struct OpExp : public Exp_ {
  Exp l;
  OpType op;
  Exp r;
  OpExp(Exp l, OpType op, Exp r);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};

struct ParenExp : public Exp_ {
  Exp exp;
  ParenExp(Exp expr);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};

struct NumExp : public Exp_ {
  int num;
  NumExp(int n);

  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};
struct TFExp : public Exp_ {
  bool val;
  TFExp(bool value);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
  Value interp(Context &c) override;
};

} // namespace libcomp
