#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/specialform.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::runtime::specialform {
DefineMacro::DefineMacro(const ast::List& list)
  : SpecialForm(list)
{
    if (list.size() != 4)
        throw BadSyntax(form_name());

    _name_list = Interpreter::get_name_list(list.craw()[2], form_name());

    _expr = list.craw()[3];

    auto& name_expr = list.craw()[1];
    if (name_expr.type() != ast::ASTNodeType::Atom ||
        name_expr.c_cast<ast::Atom>().atom_type() != ast::AtomType::Symbol)
        throw BadSyntax(form_name());

    _name = name_expr.c_cast<ast::Atom>().content();
}

Value
DefineMacro::operator()(Environment &env) 
{
    env.set<Macro>(_name, Macro(_name_list, _expr));
    return Unspecific();
}
}