#include <stdexcept>
#include <vector>
#include <libcomp/libcomp.h>


using namespace typecheck;


std::vector<Type> TYPELIST = {Type("Boolean"), Type("Int")};

	// Scope(str id, Type t, int value, Scope* n) : id(Iden(id,t,value)), next(n) {};

Scope* update(Scope* s, str i, Type t, int v) {
	return new Scope(i, t, v, s);
}
Iden lookup(Scope* s, str id){
	for (; s != nullptr; s = s->next) {
		if (s->id.id == id) {
			return s->id;
		}
	}
	throw std::runtime_error("id "+ id + " bestaat niet in scope");
}







