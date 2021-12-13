#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/macro_expander.h"
#include "libdocscript/runtime/specialform.h"
#include <limits>
#include <string>
#include <vector>

namespace libdocscript {

// +--------------------+
//      Constructor
// +--------------------+

Interpreter::Interpreter(runtime::Environment& env) : _env(env) {}

// +--------------------+
//   Public Functions
// +--------------------+

runtime::Value Interpreter::eval(const ast::Expression& expr)
{
    if (expr.type() == ast::ASTNodeType::Atom) {
        return eval_atom(expr.c_cast<ast::Atom>());
    }
    else {
        return eval_list(expr.c_cast<ast::List>());
    }
}

runtime::Value Interpreter::eval_atom(const ast::Atom& atom)
{
    switch (atom.atom_type()) {
    case ast::AtomType::Nil:
        return eval_nil(atom);
    case ast::AtomType::Symbol:
        return eval_symbol(atom);
    case ast::AtomType::Boolean:
        return eval_boolean(atom);
    case ast::AtomType::String:
        return eval_string(atom);
    case ast::AtomType::Number:
        return eval_number(atom);
    default:
        throw BadSyntax(atom.content());
    }
}

runtime::Nil Interpreter::eval_nil(const ast::Atom& atom)
{
    return runtime::Nil();
}

runtime::Value Interpreter::eval_symbol(const ast::Atom& atom)
{
    auto content = atom.content();
    auto find_res = _env.find(content);
    if (find_res == runtime::EnvironmentFindResult::ExistValue) {
        return _env.get<runtime::Value>(content);
    }
    else if (find_res == runtime::EnvironmentFindResult::ExistMacro) {
        throw BadSyntax(content);
    }
    else {
        throw UnboundedSymbol(content);
    }
}

runtime::Boolean Interpreter::eval_boolean(const ast::Atom& atom)
{
    if (atom.content() == "t") {
        return runtime::Boolean(true);
    }
    else {
        return runtime::Boolean(false);
    }
}

runtime::String Interpreter::eval_string(const ast::Atom& atom)
{
    return runtime::String(atom.content());
}

runtime::Number Interpreter::eval_number(const ast::Atom& atom)
{
    auto content = atom.content();
    if (content == "inf" || content == "+inf") {
        return runtime::Number(
            std::numeric_limits<runtime::Number::dec_type>::infinity());
    }
    if (content == "-inf") {
        return runtime::Number(
            -1 * std::numeric_limits<runtime::Number::dec_type>::infinity());
    }
    if (content == "nan" || content == "+nan") {
        return runtime::Number(
            std::numeric_limits<runtime::Number::dec_type>::signaling_NaN());
    }
    if (content == "-nan") {
        return runtime::Number(
            -1 *
            std::numeric_limits<runtime::Number::dec_type>::signaling_NaN());
    }
    if (content.find('.') == content.npos) {
        return runtime::Number(std::stoll(content));
    }
    else {
        return runtime::Number(std::stold(content));
    }
}

runtime::Value Interpreter::eval_list(const ast::List& list)
{
    if (list.is_textlist) {
        return eval_textlist(list);
    }
    else {
        auto& first = *(list.cbegin());
        if (first.type() == ast::ASTNodeType::Atom) {
            auto& atom = first.c_cast<ast::Atom>();
            auto type = atom.atom_type();
            // Not Symbol or SFOP at the first of list
            // SFOP : Special form operator
            if (type == ast::AtomType::Boolean || type == ast::AtomType::Nil ||
                type == ast::AtomType::Number || type == ast::AtomType::String)
            {
                throw NotProcedure();
            }
            // Symbol at the first of list
            if (atom.atom_type() == ast::AtomType::Symbol) {
                // If symbol bound a macro : Macro Expanding
                if (_env.find<runtime::Macro>(atom.content())) {
                    auto expr = expand_macro(list);
                    return eval(expr);
                }
                // Otherwise
                else {
                    return eval_procedure_calling(list);
                }
            }
            // Special Form Operators
            if (atom.atom_type() != ast::AtomType::Symbol) {
                return eval_special_form(list);
            }
        }
        return eval_procedure_calling(list);
    }
}

runtime::List Interpreter::eval_textlist(const ast::List& list)
{
    runtime::List result;
    for (const auto& e : list.craw()) {
        result.push_back(eval(e));
    }
    return result;
}

runtime::Value Interpreter::eval_procedure_calling(const ast::List& list)
{
    auto first_elem = eval(*(list.cbegin()));
    if (first_elem.type() != runtime::DataType::Kind::Procedure)
        throw NotProcedure();

    runtime::Procedure::args_list args;

    for (auto beg = list.cbegin() + 1; beg != list.cend(); ++beg) {
        auto arg = eval(*beg);
        if (arg.type() == runtime::DataType::Kind::Unspecific) {
            throw DefinitionException();
        }
        else {
            args.push_back(arg);
        }
    }

    return first_elem.cast<runtime::Procedure>().invoke(args, _env);
}

runtime::Value Interpreter::eval_special_form(const ast::List& list)
{
    auto sf_op = list.cbegin()->c_cast<ast::Atom>().atom_type();
    switch (sf_op) {
    case ast::AtomType::Define:
        return runtime::specialform::Define(list)(_env);

    case ast::AtomType::Set:
        return runtime::specialform::Set(list)(_env);

    case ast::AtomType::Lambda:
        return runtime::specialform::Lambda(list)(_env);

    case ast::AtomType::LogicAnd:
        return runtime::specialform::LogicAnd(list)(_env);

    case ast::AtomType::LogicOr:
        return runtime::specialform::LogicOr(list)(_env);

    case ast::AtomType::IfElse:
        return runtime::specialform::IfElse(list)(_env);

    case ast::AtomType::Quote:
        return runtime::specialform::Quote(list)(_env);

    case ast::AtomType::Quasiquote:
        return runtime::specialform::Quasiquote(list)(_env);

    case ast::AtomType::Unquote:
        // runtime::specialform::Unquote(list);
        throw UnquoteInvalid("not in quasiquote");
        break;

    case ast::AtomType::UnquoteSplicing:
        // runtime::specialform::UnquoteSplicing(list);
        throw UnquoteSplicingInvalid("not in quasiquote");
        break;

    case ast::AtomType::DefineMacro:
        return runtime::specialform::DefineMacro(list)(_env);

    default:
        throw InternalUnimplementException(
            "Interpreter::eval_special_form(), unkonwn special form: " +
            list.cbegin()->c_cast<ast::Atom>().content());
    }
}

// +-----------------------+
//         Macro
// +-----------------------+

ast::Expression Interpreter::expand_macro(const ast::List& list)
{
    auto macro_name = list.cbegin()->c_cast<ast::Atom>().content();
    auto& macro = _env.get<runtime::Macro>(macro_name);

    return runtime::MacroExpander::expand_1(
        macro,
        runtime::MacroExpander::args_list(list.cbegin() + 1, list.cend()),
        _env);
}

// +--------------------+
//   Static Functions
// +--------------------+

std::vector<std::string>
Interpreter::get_name_list(const ast::Expression& node,
                           const std::string& parent_form_name)
{
    std::vector<std::string> result;
    if (node.type() == ast::ASTNodeType::Atom) {
        auto& atom = node.c_cast<ast::Atom>();
        if (atom.atom_type() == ast::AtomType::Symbol) {
            result.push_back(atom.content());
            return result;
        }
        if (atom.atom_type() == ast::AtomType::Nil) {
            return result;
        }
        throw BadSyntax(parent_form_name);
    }
    else {
        auto& list = node.c_cast<ast::List>();
        for (const auto& elem : list.craw()) {
            if (elem.type() != ast::ASTNodeType::Atom ||
                elem.c_cast<ast::Atom>().atom_type() != ast::AtomType::Symbol)
            {
                throw BadSyntax(parent_form_name);
            }
            result.push_back(elem.c_cast<ast::Atom>().content());
        }
        return result;
    }
}

} // namespace libdocscript