
#pragma once
#include <memory>
#include <string>
#include <vector>

namespace util{

typedef std::string str;

enum class Kind { Int, Bool, Void, Func, Error, Any };

struct NameType {
	str name;
	Kind kind;

	NameType(str n, Kind k): name(n), kind(k){};
	str toString() const;
	bool operator==(const NameType& p) const;
};



struct Type {
   Kind kind;
   std::vector<NameType> params;
   std::shared_ptr<Type> ret;

   explicit Type(Kind k = Kind::Error) : kind(k) {}
   static Type Int() { return Type(Kind::Int); }
   static Type Any() { return Type(Kind::Any); }
   static Type Bool() { return Type(Kind::Bool); }
   static Type Void() { return Type(Kind::Void); }
   static Type Error() { return Type(Kind::Error); }
   static Type Func(std::vector<NameType> params, Type ret);

   bool operator==(const Type& o) const;
   bool operator!=(const Type& o) const { return !(*this == o); }
   str toString() const;
};


template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// C++17 only: deduction guide so the compiler can infer Ts from the constructor arguments
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


}
