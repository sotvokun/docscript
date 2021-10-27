#ifndef DOCSIR_STDPROC_EXT_H
#define DOCSIR_STDPROC_EXT_H
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/value.h"
#include "docsir/stdproc.h"
#include "docsir/environment.h"

namespace docscript::docsir::stdproc {

// IO

Value write_file(const argument_type &args);
Value read_file(const argument_type &args);
Value message(const argument_type &args);

// Environment

BuiltInProcedure::function_type privilege_load(Environment &env);

// Interpretation

Value error(const argument_type &args);
Value exit(const argument_type &args);

}
#endif