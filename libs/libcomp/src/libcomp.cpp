#include "libcomp/libcomp.h"
#include <iostream>
namespace libcomp {

Expl result = nullptr;

NumExp::NumExp(int n) : num(n){};
void NumExp::debug() {
   std::cout << num;
}

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

ParenExp::ParenExp(Exp expr): exp(expr) {};
void ParenExp::debug() {
   std::cout << '(';
   exp->debug();
   std::cout << ')';
}

LineExp::LineExp(Exp e, Expl t) : exp(e), tail(t) {};
void LineExp::debug(){
   tail->debug();
   std::cout << std::endl;
   exp->debug();
}

EndExp::EndExp(Exp e) : exp(e) {};
void EndExp::debug(){
   exp->debug();
}



} // namespace libcomp
