#include "libcomp/libcomp.h"
#include <iostream>
namespace libcomp {

using namespace typecheck;

/*
* Literals
*/
NumExp::NumExp(int n) : num(n){};
void NumExp::debug() {
   std::cout << num;
}
Type NumExp::evaltype() {
   std::cout << "Int";
   return Type("Int");
};

TFExp::TFExp(bool val): val(val){};
void TFExp::debug(){
   if (val) {
      std::cout << "true";
   } else {
      std::cout << "false";
   }
};
Type TFExp::evaltype() {
   std::cout << "TF";
   return Type("Boolean");
};



/* 
*
*/
char debugOpExp(OpType op) {
   switch (op) {
      case Plus: { return '+'; }
      case Minus: { return '-'; }
      case Multiply: { return '*'; }
      case Divide: { return '/'; }
   }
   exit(1);
}
OpExp::OpExp(Exp l, OpType op, Exp r): l(l), op(op), r(r) {};
void OpExp::debug() {
   l->debug();
   std::cout << debugOpExp(op);
   r->debug();
}
Type OpExp::evaltype() {
   Type lt = l->evaltype();
   std::cout << debugOpExp(op);
   Type rt = r->evaltype();
   if (lt.name != rt.name) {
      typeErrorFound();
   }
   return lt;
};

const char* debugBoolExpType(BoolExpType t){
   switch (t) {
      case Eq:  { return "=="; }
      case Neq: { return "!="; }
      case Le:  { return ">="; }
      case Lt:  { return ">"; }
      case Ge:  { return "<="; }
      case Gt:  { return "<"; }
      case And: { return "&&"; }
      case Or:  { return "||"; }
   }
   exit(1);
}
BoolExp::BoolExp(Exp left, BoolExpType type, Exp right): l(left), r(right), t(type){};
void BoolExp::debug() {
   l->debug();
   std::cout << debugBoolExpType(t);
   r->debug();
}
Type BoolExp::evaltype() {
   Type lt = l->evaltype();
   std::cout  << debugBoolExpType(t); 
   Type rt = r->evaltype();
   //TODO checken of type wel comparable is ofzo?
   if (lt.name != rt.name) {
      typeErrorFound();
   }
   return Type("Boolean");
};

ParenExp::ParenExp(Exp expr): exp(expr) {};
void ParenExp::debug() {
   std::cout << '(';
   exp->debug();
   std::cout << ')';
}
Type ParenExp::evaltype() {
   std::cout << '(' ;
   Type t = exp->evaltype();
   std::cout << ')';
   return t;
}

LineExp::LineExp(Exp e, Expl t) : exp(e), tail(t) {};
void LineExp::debug(){
   tail->debug();
   std::cout << ";" << std::endl;
   exp->debug();
}
Type LineExp::evaltype() {
   tail->evaltype();
   std::cout << std::endl;
   return exp->evaltype();
}

EndExp::EndExp(Exp e) : exp(e) {};
void EndExp::debug() {
   exp->debug();
}
Type EndExp::evaltype() {
    Type t = exp->evaltype();
   return t;
}



} // namespace libcomp
