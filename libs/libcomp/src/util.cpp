#include "libcomp/util.h"
#include <format>

using namespace util;

str kindToString(Kind kind){
    switch (kind) {
			case Kind::Int:   return "Int";
			case Kind::Bool:  return "Bool";
			case Kind::Void:  return "Void";
			case Kind::Any:  return "Any";
			case Kind::Error: return "<error>";
			case Kind::Func: return "<func>";
    }
    return "?";
}

Type Type::Func(std::vector<NameType> params, Type ret) {
    Type t(Kind::Func);
    t.params = std::move(params);
    t.ret = std::make_shared<Type>(std::move(ret));
    return t;
}
str NameType::toString() const {
	return std::format("{}:{}", name, kindToString(kind));
}
bool NameType::operator==(const NameType& p) const{
	return name == p.name && kind == p.kind;
}

bool Type::operator==(const Type& o) const {
    if (kind != o.kind) return false;
    if (kind != Kind::Func) return true;
    return params == o.params && *ret == *o.ret;
}
str Type::toString() const {
	if (kind != Kind::Func) return kindToString(kind);

	str s = "(";
	for (size_t i = 0; i < params.size(); i++)
			s += (i ? ", " : "") + params[i].toString();
	return s + ") -> " + ret->toString();
}

