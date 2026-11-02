#include <string>
#include <unordered_map>
#include <variant>

namespace interp {

typedef std::string str;

struct Value {
	std::variant<std::monostate, int, bool> val;

	static Value Void() { return {std::monostate{}}; }
	static Value Int(int i) { return {i}; }
	static Value Bool(bool b) { return {b}; }

	// als dit ooit bad_variant_access throwed, is er een typecheck fout
	int asInt() { return std::get<int>(val);}
	bool asBool() { return std::get<bool>(val);}

};


class Context {
private:
   std::unordered_map<str, Value> values_;
   Context *parent_;

private: 
	Context* findContext(str id);

public:
   Context(Context *parent = nullptr) : parent_(parent) {}

   // true/false volgens of symbol al bestaat
   void declare(str id, Value val);
   void assign(str id, Value val);
   Value lookup(str &name);
   void error(str out);
};



} // ns interp
