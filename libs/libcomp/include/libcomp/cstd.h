
#pragma once
#include "libcomp/interp.h"
#include "libcomp/typecheck.h"

namespace cstd {


void loadStd(typecheck::Scope& root);
void loadStd(interp::Context& root);


}
