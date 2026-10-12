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
OpExp::OpExp(Exp l, OpType op, Exp r): l(l), op(op), r(r) {};
void OpExp::debug() {
   l->debug();
   switch (op) {
      case Plus: {
         std::cout << '+';
         break;
      }
      case Minus: {
         std::cout << '-';
         break;
      }
      case Multiply: {
         std::cout << '*';
         break;
      }
      case Divide: {
         std::cout << '/';
         break;
      }
   }
   r->debug();
}
BoolExp::BoolExp(Exp left, BoolExpType type, Exp right): l(left), r(right), t(type){};
void BoolExp::debug() {
   l->debug();
   switch (t) {
      case Eq:  { std::cout << "=="; break;}
      case Neq: { std::cout << "!="; break;}
      case Le:  { std::cout << ">="; break;}
      case Lt:  { std::cout << ">"; break;}
      case Ge:  { std::cout << "<="; break;}
      case Gt:  { std::cout << "<"; break;}
      case And: { std::cout << "&&"; break;}
      case Or:  { std::cout << "||"; break;}

   }
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
void EndExp::debug(){
   exp->debug();
}



} // namespace libcomp
