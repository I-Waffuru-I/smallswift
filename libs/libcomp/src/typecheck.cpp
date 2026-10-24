#include <stdexcept>
#include <utility>
#include <vector>
#include "libcomp/libcomp.h"
#include "libcomp/typecheck.h"


using namespace typecheck;

bool Scope::declare(str& name, Symbol sym) {
	// returns een pair<iter, bool>
	return symbols_.emplace(name, std::move(sym)).second;
};
const Symbol* Scope::lookup(str& name){
    for (const Scope* s = this; s; s = s->parent_) {
		auto it = s->symbols_.find(name);
        if (it != s->symbols_.end())
			return &it->second;
    }
    return nullptr;
};


// // scope is een list aan defined values
// struct Scope {
// 	Iden id;
// 	Scope* next;
// 	Scope(Iden id, Scope* n) : id(id), next(n) {};
// };
// Scope* update(Scope* s, str i, Type t, int v);
// int lookup(Scope* s, str id);


bool TChecker::run(const std::vector<libcomp::Stm>& program){
	return false;
};

Type TChecker::check(libcomp::Exp e){
	return Type::Void();
};
void TChecker::check(libcomp::Stm s){

};
