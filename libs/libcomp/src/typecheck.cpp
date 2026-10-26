#pragma once
#include <stdexcept>
#include <utility>
#include <vector>
#include "libcomp/libcomp.h"
#include "libcomp/typecheck.h"


using namespace typecheck;

Type Type::Func(std::vector<Type> params, Type ret) {
    Type t(Kind::Func);
    t.params = std::move(params);
    t.ret = std::make_shared<Type>(std::move(ret));
    return t;
}

bool Type::operator==(const Type& o) const {
    if (kind != o.kind) return false;
    if (kind != Kind::Func) return true;
    return params == o.params && *ret == *o.ret;
}

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

