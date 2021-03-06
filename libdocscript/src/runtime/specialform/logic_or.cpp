#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/specialform.h"
#include "libdocscript/runtime/value.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/ast/list.h"

namespace libdocscript::runtime::specialform {
LogicOr::LogicOr(const ast::List& list)
  : SpecialForm(list)
  , _exprs(list.cbegin(), list.cend())
{}

Value
LogicOr::operator()(Environment& env)
{
    Value result = Boolean(false);

    for(const auto &expr : _exprs) {
        auto value = Interpreter(env).eval(expr);
        if (value.type() == DataType::Kind::Unspecific) {
            throw DefinitionException();
        }

        if(static_cast<bool>(value)) {
            return value;
        } else {
            result = value;
        }
    }
    return result;
}
}