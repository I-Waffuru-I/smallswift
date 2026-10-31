#pragma once
#include "libcomp/typecheck.h"
namespace libcomp {

using namespace typecheck;


struct Stm_ {
  virtual ~Stm_() {};
  virtual void debug() = 0;
  virtual void check(Scope &s) = 0;
};
struct Exp_ {
  virtual ~Exp_() {}
  virtual void debug() = 0;
  virtual Type check(Scope &s) = 0;
};

typedef Stm_ *Stm;
typedef Exp_ *Exp;



struct StmList : public Stm_ {
  std::vector<Stm> stms;
  void add(Stm s) { stms.push_back(s); }
  void debug() override;
  void check(Scope &s) override;
};
struct ExpStm : public Stm_ {
  Exp exp;
  ExpStm(Exp e): exp(e){};
  void debug() override;
  void check(Scope &s) override;
};



struct DeclareStm : public Stm_ {
	str id;
	Exp val;
	bool mut;
	DeclareStm(str id, Exp v, bool m): id(id), val(v), mut(m) {};
	void debug() override;
	void check(Scope &s) override;
};
struct AssignStm : public Stm_ {
	str id;
	Exp val;
	AssignStm(str id, Exp v): id(id), val(v) {};
	void debug() override;
	void check(Scope &s) override;
};
struct VariableExp : public Exp_ {
	str id;
	VariableExp(str id): id(id) {};
	void debug() override;
	Type check(Scope &s) override;
};



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



struct FunDeclare: public Stm_ {
	str name;
	std::vector<Type> params;
	Type rtype;
	FunDeclare(str name, std::vector<Type> params, Type ret): name(name), params(params), rtype(ret) {} 
	void debug() override;
	void check(Scope &s) override;
};
struct FunCall: public Exp_ {

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


} // namespace libcomp
