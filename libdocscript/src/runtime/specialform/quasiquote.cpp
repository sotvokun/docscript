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
Quasiquote::Quasiquote(const ast::List& list)
  : SpecialForm(list)
{
    if (list.size() != 2)
        throw BadSyntax(form_name());

    _expr = list.craw()[1];
}

Value
Quasiquote::operator()(Environment& env)
{
    return Quasiquote::process(_expr, env, 1);
}

// +--------------------+
//    Static Functions
// +--------------------+

Value
Quasiquote::process(const ast::Expression& expr, Environment& env, int depth)
{
    if (expr.type() == ast::ASTNodeType::Atom) {
        return Quote::process_atom(expr.c_cast<ast::Atom>(), env);
    } else {
        return Quasiquote::process_list(expr.c_cast<ast::List>(), env, depth);
    }
}

Value
Quasiquote::process_list(const ast::List& list, Environment& env, int depth)
{
    if (is_unquote(list)) {
        return Unquote(list)(env);
    }

    if (is_splicing(list)) {
        if (depth == 1) {
            throw UnquoteSplicingInvalid("invalid context within quasiquote");
        } else {
            return UnquoteSplicing(list)(env);
        }
    }

    List result;
    bool atom_unsplicing = false;
    for (const auto& expr : list.craw()) {
        auto value = process(expr, env, depth + 1);
        if (expr.type() == ast::ASTNodeType::Atom) {
            result.push_back(value);
        } else {
            auto& list_expr = expr.c_cast<ast::List>();
            // Expand the list of the unquote-spcling result
            if (is_splicing(list_expr)) {
                auto inner_value =
                  UnquoteSplicing::expose_list(value.c_cast<List>());
                if (inner_value.type() == DataType::Kind::List) {
                    for (const auto& val : inner_value.c_cast<List>().craw())
                        result.push_back(val);
                } else {
                    atom_unsplicing = true;
                    result.push_back(inner_value);
                }
            } else {
                result.push_back(value);
            }
        }
    }

        return result;
}

bool
Quasiquote::is_unquote(const ast::List& list)
{
    return list.size() && list.cbegin()->type() == ast::ASTNodeType::Atom &&
           list.cbegin()->c_cast<ast::Atom>().atom_type() ==
             ast::AtomType::Unquote;
}

bool
Quasiquote::is_splicing(const ast::List& list)
{
    return list.size() && list.cbegin()->type() == ast::ASTNodeType::Atom &&
           list.cbegin()->c_cast<ast::Atom>().atom_type() ==
             ast::AtomType::UnquoteSplicing;
}
}