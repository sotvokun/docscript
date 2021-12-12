#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/macro.h"
#include <algorithm>
#include <string>


namespace libdocscript::runtime {

using namespace ast;

// +-------------------+
//      Constructor
// +-------------------+

Macro::Macro(const param_list& names, ast::Expression expr)
    : _params(names), _expression(expr)
{}

// +-------------------+
//    Public Functions
// +-------------------+

const Macro::param_list& Macro::parameters() const
{
    return _params;
}
} // namespace libdocscript::runtime