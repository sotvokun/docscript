#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/specialform.h"
#include "libdocscript/runtime/value.h"


namespace libdocscript::runtime::specialform {
Lambda::Lambda(const ast::List& list) : SpecialForm(list)
{
    if (list.size() != 3) throw BadSyntax(form_name());

    _expr = list.craw()[2];

    _name_list = Interpreter::get_name_list(list.craw()[1], form_name());
}

Value Lambda::operator()(Environment& env)
{
    return Value(LambdaProcedure(_name_list, _expr));
}
} // namespace libdocscript::runtime::specialform