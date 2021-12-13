#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/specialform.h"
#include "libdocscript/runtime/value.h"


namespace libdocscript::runtime::specialform {
Define::Define(const ast::List& list) : SpecialForm(list)
{
    if (list.size() != 3) throw BadSyntax(form_name());

    _value_expr = list.craw()[2];

    auto& elem = list.craw()[1];
    if (elem.type() != ast::ASTNodeType::Atom &&
        elem.c_cast<ast::Atom>().atom_type() != ast::AtomType::Symbol)
        throw BadSyntax(form_name());

    _name = elem.c_cast<ast::Atom>().content();
}

Value Define::operator()(Environment& env)
{
    auto val = Interpreter(env).eval(_value_expr);
    if (val.type() == DataType::Kind::Unspecific) {
        throw DefinitionException();
    }

    env.set(_name, val);
    return Unspecific();
}
} // namespace libdocscript::runtime::specialform
