#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/specialform.h"
#include "libdocscript/runtime/value.h"
#include "libdocscript/runtime/list.h"

namespace libdocscript::runtime::specialform {
Unquote::Unquote(const ast::List &list)
  : SpecialForm(list)
{
    if (list.size() != 2)
        throw BadSyntax(form_name());

    _expr = list.craw()[1];
}

Value
Unquote::operator()(Environment &env)
{
    return Interpreter(env).eval(_expr);
}


}