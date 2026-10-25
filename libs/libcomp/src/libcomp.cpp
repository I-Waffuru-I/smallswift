#pragma once
#include "libcomp/libcomp.h"
#include <iostream>
namespace libcomp {

/*
* Literals
*/
NumExp::NumExp(int n) : num(n){};
void NumExp::debug() {
   std::cout << num;
}

TFExp::TFExp(bool val): val(val){};
void TFExp::debug(){
   if (val) {
      std::cout << "true";
   } else {
      std::cout << "false";
   }
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
Type OpExp::check(Scope& s) {
   Type lt = l->check(s);
   Type rt = r->check(s);

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

ParenExp::ParenExp(Exp expr): exp(expr) {};
void ParenExp::debug() {
   std::cout << '(';
   exp->debug();
   std::cout << ')';
}

LineExp::LineExp(Exp e, Expl t) : exp(e), tail(t) {};
void LineExp::debug(){
   tail->debug();
   std::cout << ";" << std::endl;
   exp->debug();
}

EndExp::EndExp(Exp e) : exp(e) {};
void EndExp::debug() {
   exp->debug();
}



} // namespace libcomp
