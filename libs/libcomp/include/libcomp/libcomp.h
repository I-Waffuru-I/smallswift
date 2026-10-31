#pragma once
#include "libcomp/typecheck.h"
namespace libcomp {

using namespace typecheck;


struct Stm_ {
  virtual ~Stm_() {};
  virtual void debug(size_t depth) = 0;
  virtual void check(Scope &s) = 0;
};
struct Exp_ {
  virtual ~Exp_() {}
  virtual void debug(size_t depth) = 0;
  virtual Type check(Scope &s) = 0;
};

typedef Stm_ *Stm;
typedef Exp_ *Exp;



struct StmList : public Stm_ {
  std::vector<Stm> stms;
  void add(Stm s) { stms.push_back(s); }
  void debug(size_t depth) override;
  void check(Scope &s) override;
};
struct ExpStm : public Stm_ {
  Exp exp;
  ExpStm(Exp e): exp(e){};
  void debug(size_t depth) override;
  void check(Scope &s) override;
};
struct BlockStm: public Stm_ {
   StmList* stms;
   Type rtype;
   BlockStm(StmList* s): stms(s){ rtype = Type::Void(); };
   BlockStm(StmList*s, Type r): stms(s), rtype(r){};
   void debug(size_t depth) override;
   void check(Scope &s) override;
};

struct DoBlockStm: public Stm_ {
   Stm block;
   DoBlockStm(Stm s): block(s){ };
   void debug(size_t depth) override;
   void check(Scope &s) override;
};
struct IfStm : public Stm_ {
   Exp cond;
   Stm then;
   Stm els;
   IfStm(Exp c, Stm s, Stm e = nullptr): cond(c), then(s), els(e) {};
   void debug(size_t depth) override;
   void check(Scope &s) override;
};




struct FunDeclare: public Stm_ {
	str name;
	std::vector<Type> params;
	Type rtype;
	FunDeclare(str name, std::vector<Type> params, Type ret): name(name), params(params), rtype(ret) {} 
	void debug(size_t depth) override;
	void check(Scope &s) override;
};
struct FunCall: public Exp_ {

  void debug(size_t depth) override;
  Type check(Scope &s) override;
};






struct DeclareStm : public Stm_ {
	str id;
	Exp val;
	bool mut;
	DeclareStm(str id, Exp v, bool m): id(id), val(v), mut(m) {};
	void debug(size_t depth) override;
	void check(Scope &s) override;
};
struct AssignStm : public Stm_ {
	str id;
	Exp val;
	AssignStm(str id, Exp v): id(id), val(v) {};
	void debug(size_t depth) override;
	void check(Scope &s) override;
};
struct VariableExp : public Exp_ {
	str id;
	VariableExp(str id): id(id) {};
	void debug(size_t depth) override;
	Type check(Scope &s) override;
};







enum OpType { Plus, Minus, Multiply, Divide };
enum BoolExpType { And, Or, Eq, Neq, Lt, Gt, Le, Ge };
struct BoolExp : public Exp_ {
  Exp l, r;
  BoolExpType t;
  BoolExp(Exp left, BoolExpType type, Exp right);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
};
struct OpExp : public Exp_ {
  Exp l;
  OpType op;
  Exp r;
  OpExp(Exp l, OpType op, Exp r);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
};

struct ParenExp : public Exp_ {
  Exp exp;
  ParenExp(Exp expr);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
};


struct NumExp : public Exp_ {
  int num;
  NumExp(int n);

  void debug(size_t depth) override;
  Type check(Scope &s) override;
};
struct TFExp : public Exp_ {
  bool val;
  TFExp(bool value);
  void debug(size_t depth) override;
  Type check(Scope &s) override;
};




} // namespace libcomp
