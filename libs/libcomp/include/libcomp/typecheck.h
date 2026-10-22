#include <iostream>
#include <string>
namespace typecheck {

typedef std::string str;



struct Type {
	str name;
	Type(str n): name(n){}
};
struct Iden {
	str id;
	Type type;
	int value;

	Iden(str id, Type t, int val) : id(id), type(t), value(val) {}
};




// scope is een list aan defined values
struct Scope {
	Iden id;
	Scope* next;
	Scope(str id, Type t, int value, Scope* n) : id(Iden(id,t,value)), next(n) {};
};
Scope* update(Scope* s, str i, Type t, int v);
int lookup(Scope* s, str id);


static void typeErrorFound(){
	std::cerr << "Incompatible type" << std::endl;
	exit(1);
}


}
