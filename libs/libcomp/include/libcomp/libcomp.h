#pragma once
namespace libcomp {


struct Stm_ { 
	virtual ~Stm_(){};
	virtual void debug(){};
};
struct Exp_ {
	virtual ~Exp_(){}
	virtual void debug(){};
};

typedef Stm_* Stm;
typedef Exp_* Exp;

// waar bison naar output
// defined in libcomp.cpp
extern Stm result;


struct NumExp : public Exp_ {
	int num;
	NumExp(int n);
	void debug();
};
enum OpType {
	Plus,
	Minus,
	Multiply,
	Divide
};
struct OpExp : public Exp_ {
	Exp l;
	OpType op;
	Exp r;
	OpExp(Exp l, OpType op, Exp r);
	void debug();
};
struct ParenExp : public Exp_ {
	Exp exp;
	ParenExp(Exp expr);
	void debug();
};


} // namespace libcomp
