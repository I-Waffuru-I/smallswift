#pragma once
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
