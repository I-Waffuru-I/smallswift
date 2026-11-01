
#include "libcomp/interp.h"
#include <cassert>
#include <iostream>
using namespace interp;

// true/false volgens of symbol al bestaat
void Context::declare(str id, Value val){

	values_[id] = val;
};

Context* Context::findContext(str id){
    for (Context* c = this; c; c = c->parent_) {
		auto it = c->values_.find(id);
        if (it != c->values_.end())
			return c;
    }
	return nullptr;
}
void Context::assign(str id, Value val) {
	Context* c = findContext(id);
	if (!c) {
		error("Identifier ["+id+"] is not found in any scope at runtime");
	}
	c->values_[id] = val;
}

const Value* Context::lookup(str &name){
    for (const Context* s = this; s; s = s->parent_) {
		auto it = s->values_.find(name);
        if (it != s->values_.end())
			return &it->second;
    }
    return nullptr;

};

void Context::error(str out){
   std::cerr << out << std::endl;
   exit(1);
};
