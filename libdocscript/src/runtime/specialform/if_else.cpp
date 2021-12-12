#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/specialform.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::runtime::specialform {
IfElse::IfElse(const ast::List& list) : SpecialForm(list)
{
    if (list.size() != 4) throw BadSyntax(form_name());

    _cond = list.craw()[1];
    _then = list.craw()[2];
    _else = list.craw()[3];
}

Value IfElse::operator()(Environment& env)
{
    auto interpreter = Interpreter(env);
    auto cond_result = interpreter.eval(_cond);

    if (cond_result.type() == DataType::Kind::Unspecific)
        throw DefinitionException();

    if (static_cast<bool>(cond_result)) {
        auto result = interpreter.eval(_then);
        return SpecialForm::check_value_validation(result);
    }
    else {
        auto result = interpreter.eval(_else);
        return SpecialForm::check_value_validation(result);
    }
}
} // namespace libdocscript::runtime::specialform