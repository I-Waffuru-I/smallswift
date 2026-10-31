#pragma once
#include "libcomp/libcomp.h"
#include "libcomp/typecheck.h"
#include <cstddef>
#include <iostream>
#include <vector>
namespace libcomp {

// util om te printen tijdens debug
void indent(size_t depth){
   for (size_t d = 0; d < depth; d++)
      std::cout << "   ";
}

/*
 * high level crap
 */
void StmList::debug(size_t depth) {
   for (Stm s : stms) {
      indent(depth);
      s->debug(depth);
      std::cout << ";" << std::endl;
   }
};
void StmList::check(Scope &s) {
   for (Stm st : stms) {
      st->check(s);
   }
};

void ExpStm::debug(size_t depth) {
   indent(depth);
   exp->debug(depth);
};
void ExpStm::check(Scope &s) { exp->check(s); };





/*
 * blocks
 */
void BlockStm::debug(size_t depth) {
   std::cout << "{" << std::endl;
   stms->debug(depth + 1);
   std::cout << "}";
};
void BlockStm::check(Scope &s) {
   Scope inner(s);
   stms->check(inner);
};

void DoBlockStm::debug(size_t depth) {
   std::cout << "do ";
   block->debug(depth);
};
void DoBlockStm::check(Scope &s) { block->check(s); };
void IfStm::debug(size_t depth) {
   std::cout << "if ";
   cond->debug(depth);
   then->debug(depth);
   if (els){
      std::cout << " else ";
      els->debug(depth);
   }
};
void IfStm::check(Scope &s) {
   Type c = cond->check(s);
   if (c.kind != typecheck::Type::Kind::Bool) {
      std::cerr << "Expected bool type in condition, not [" << c.toString() << "]"
         << std::endl;
      exit(1);
   }
   then->check(s);
   if (els)
      els->check(s);
};

/*
 * functions
 */
void FunCall::debug(size_t depth) {

};
Type FunCall::check(Scope &_) {
   std::vector<Type> params;
   return Type::Func(params, Type::Bool());
};

/*
 * variables
 */
void DeclareStm::debug(size_t depth) {
   if (mut) {
      std::cout << "var ";
   } else {
      std::cout << "let ";
   }
   std::cout << id << " = ";
   val->debug(depth);
};
void DeclareStm::check(Scope &s) {
   const Symbol *prev = s.lookup(id);
   if (prev) {
      std::cerr << "Invalid redeclaration of [" << id << "]" << std::endl;
      exit(1);
   }
   Type t = val->check(s);
   Symbol sym = {t, mut};
   s.declare(id, sym);
};
void AssignStm::debug(size_t depth) {
   std::cout << id << " = ";
   val->debug(depth);
};
void AssignStm::check(Scope &s) {
   Type t = val->check(s);
   const Symbol *sy = s.lookup(id);
   if (!sy) {
      std::cerr << "Tried to set [" << id << "] but it is not declared"
         << std::endl;
      exit(1);
   } else if (!sy->isMutable) {
      std::cerr << "Tried to set [" << id << "] but it is not defined as mutable"
         << std::endl;
      exit(1);
   } else if (sy->type != t) {
      std::cerr << "Tried to set [" << id << "], can't assign [" << t.toString()
         << "] to a [" << sy->type.toString() << "]" << std::endl;
      exit(1);
   }
};
void VariableExp::debug(size_t depth) { std::cout << id; };
Type VariableExp::check(Scope &s) {
   const Symbol *sy = s.lookup(id);
   if (!sy) {
      std::cerr << "Cannot find variable [" << id << "] in scope" << std::endl;
      exit(1);
   }
   return sy->type;
};

/*
 * operations
 */
char debugOpExp(OpType op) {
   switch (op) {
      case Plus: {
         return '+';
      }
      case Minus: {
         return '-';
      }
      case Multiply: {
         return '*';
      }
      case Divide: {
         return '/';
      }
   }
   exit(1);
}
OpExp::OpExp(Exp l, OpType op, Exp r) : l(l), op(op), r(r) {};
void OpExp::debug(size_t depth) {
   l->debug(depth);
   std::cout << debugOpExp(op);
   r->debug(depth);
}
Type OpExp::check(Scope &s) {
   Type lt = l->check(s);
   Type rt = r->check(s);

   if (lt != rt) {
      std::cerr << "Mismatched types" << std::endl;
      exit(1);
   } else if (lt.kind != typecheck::Type::Kind::Int) {
      std::cerr << "Can't apply operation [" << debugOpExp(op) << "] to types ["
         << lt.toString() << "] and [" << rt.toString() << "]"
         << std::endl;
      exit(1);
   }

   return lt;
};

const char *debugBoolExpType(BoolExpType t) {
   switch (t) {
      case Eq: {
         return "==";
      }
      case Neq: {
         return "!=";
      }
      case Le: {
         return "<=";
      }
      case Lt: {
         return "<";
      }
      case Ge: {
         return ">=";
      }
      case Gt: {
         return ">";
      }
      case And: {
         return "&&";
      }
      case Or: {
         return "||";
      }
   }
   exit(1);
}
BoolExp::BoolExp(Exp left, BoolExpType type, Exp right)
: l(left), r(right), t(type) {};
void BoolExp::debug(size_t depth) {
   l->debug(depth);
   std::cout << debugBoolExpType(t);
   r->debug(depth);
}
Type BoolExp::check(Scope &s) {
   Type lt = l->check(s);
   Type rt = r->check(s);

   if (lt != rt) {
      std::cerr << "Mismatched types" << std::endl;
      exit(1);
   } else if (lt.kind == typecheck::Type::Kind::Bool && t != And && t != Or) {
      std::cerr << "Can't apply operation [" << debugBoolExpType(t)
         << "] to types [" << lt.toString() << "] and [" << rt.toString()
         << "]" << std::endl;
      exit(1);
   }
   return Type::Bool();
};

ParenExp::ParenExp(Exp expr) : exp(expr) {};
void ParenExp::debug(size_t depth) {
   std::cout << '(';
   exp->debug(depth);
   std::cout << ')';
}
Type ParenExp::check(Scope &s) { return exp->check(s); }

/*
 * Literals
 */
NumExp::NumExp(int n) : num(n) {};
void NumExp::debug(size_t depth) { std::cout << num; }
Type NumExp::check(Scope &_) { return Type::Int(); }

TFExp::TFExp(bool val) : val(val) {};
void TFExp::debug(size_t depth) {
   if (val) {
      std::cout << "true";
   } else {
      std::cout << "false";
   }
};
Type TFExp::check(Scope &_) { return Type::Bool(); };

} // namespace libcomp
