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
Quote::Quote(const ast::List &list)
  : SpecialForm(list)
{
    if(list.size() != 2)
        throw BadSyntax(form_name());

    _expr = list.craw()[1];
}

Value
Quote::operator()(Environment &env)
{
    return Quote::process(_expr, env);
}

// +--------------------+
//      Constructor
// +--------------------+

Value
Quote::process(const ast::Expression &expr, Environment& env) {
    if(expr.type() == ast::ASTNodeType::Atom) {
        return Quote::process_atom(expr.c_cast<ast::Atom>(), env);
    } else {
        return Quote::process_list(expr.c_cast<ast::List>(), env);
    }
}

Value
Quote::process_atom(const ast::Atom &atom, Environment &env) {
    auto atom_type = atom.atom_type();
    if(atom_type == ast::AtomType::Nil) {
        return Nil();
    }

    if(atom_type == ast::AtomType::Number) {
        return Interpreter(env).eval_number(atom);
    }

    if(atom_type == ast::AtomType::String) {
        return Interpreter(env).eval_string(atom);
    }

    if(atom_type == ast::AtomType::Boolean) {
        return Interpreter(env).eval_boolean(atom);
    }

    return Symbol(atom.content());
}

Value
Quote::process_list(const ast::List &list, Environment &env) {
    List result;
    for(const auto &e : list.craw()) {
        result.push_back(process(e, env));
    }
    return result;
}

}