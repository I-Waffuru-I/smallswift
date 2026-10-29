#pragma once
#include "libcomp/libcomp.h"
#include <iostream>
#include <vector>
namespace libcomp {


void StmList::debug(){
   for (Stm s : stms) {
      s->debug();
   }
};
void StmList::check(Scope &s){
   for (Stm st : stms) {
      st->check(s);
   }
};


void ExpStm::debug(){exp->debug();};
void ExpStm::check(Scope &s) {
   exp->check(s);
};


/*
* Literals
*/
NumExp::NumExp(int n) : num(n){};
void NumExp::debug() {
   std::cout << num;
}
Type NumExp::check(Scope &_){
   return Type::Int();
}

TFExp::TFExp(bool val): val(val){};
void TFExp::debug(){
   if (val) {
      std::cout << "true";
   } else {
      std::cout << "false";
   }
};
Type TFExp::check(Scope &_) {
   return Type::Bool();
};



void DeclareStm::debug(){
   if (mut) {
      std::cout << "let ";
   } else {
      std::cout << "let ";
   }
   std::cout << id << " = ";
   val->debug();
};
void DeclareStm::check(Scope &s){
   val->check(s);
};
void AssignStm::debug(){
   std::cout << id << " = ";
   val->debug();
};
void AssignStm::check(Scope &s){
   val->check(s);
};



void FunCall::debug() {

};
Type FunCall::check(Scope &_){

   std::vector<Type> params;
   return Type::Func(params, Type::Bool());
};






/* 
* operations
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

   if (lt != rt){
      std::cerr << "Mismatched types" << std::endl;
      exit(1);
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
Type BoolExp::check(Scope& s) {
   Type lt = l->check(s);
   Type rt = r->check(s);

   if (lt != rt){
      std::cerr << "Mismatched types" << std::endl;
      exit(1);
   }
   return Type::Bool();
};

ParenExp::ParenExp(Exp expr): exp(expr) {};
void ParenExp::debug() {
   std::cout << '(';
   exp->debug();
   std::cout << ')';
}
Type ParenExp::check(Scope &s){
   return exp->check(s);
}



} // namespace libcomp
