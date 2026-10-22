#include "libcomp/libcomp.h"
#include <vector>


// output expression list that contains the parsed program
libcomp::Expl result = nullptr;
// all defined types found while parsing
extern std::vector<typecheck::Type> TYPELIST;
