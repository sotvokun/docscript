#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/macro_expander.h"
#include <unordered_map>


namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

MacroExpander::MacroExpander(Macro& macro, args_map& args, Environment& env,
                             bool rec_expand)
    : _env(env), _macro(macro), _expand_recusrive(rec_expand), _args(args)
{}

// +--------------------+
//    Static Functions
// +--------------------+

ast::Expression MacroExpander::expand_1(Macro& macro, const args_list& args,
                                        Environment& env)
{
    auto map = create_map(macro.parameters(), args);
    auto expander = MacroExpander(macro, map, env, false);
    return expander.expand_expression(expander._macro._expression);
}

ast::Expression MacroExpander::expand(Macro& macro, const args_list& args,
                                      Environment& env)
{
    auto map = create_map(macro.parameters(), args);
    auto expander = MacroExpander(macro, map, env, true);
    return expander.expand_expression(expander._macro._expression);
}

MacroExpander::args_map
MacroExpander::create_map(const Macro::param_list& param,
                          const MacroExpander::args_list& args)
{
    if (param.size() != args.size()) throw MacroExpandingFailed();

    args_map map;
    for (decltype(args.size()) i = 0; i != args.size(); ++i) {
        map.insert_or_assign(param[i], args[i]);
    }
    return map;
}

bool MacroExpander::find_name(const std::string& name)
{
    return _args.find(name) != _args.end();
}

// +--------------------+
//   Private Functions
// +--------------------+

ast::Expression MacroExpander::expand_expression(const ast::Expression& expr)
{
    if (expr.type() == ast::ASTNodeType::Atom) {
        return expand_atom(expr.c_cast<ast::Atom>());
    }
    else {
        return expand_list(expr.c_cast<ast::List>());
    }
}

ast::Expression MacroExpander::expand_atom(const ast::Atom& atom)
{
    if (atom.atom_type() != ast::AtomType::Symbol || !find_name(atom.content()))
    {
        return atom;
    }
    else {
        return _args.at(atom.content());
    }
}

ast::Expression MacroExpander::expand_list(const ast::List& list)
{
    // Enable Expand Recursive : Trying to expand other macros
    if (_expand_recusrive && list.size() != 0) {
        auto first = list.cbegin();
        if (first->type() == ast::ASTNodeType::Atom &&
            first->c_cast<ast::Atom>().atom_type() == ast::AtomType::Symbol &&
            _env.find<Macro>(first->c_cast<ast::Atom>().content()))
        {
            Macro& macro =
                _env.get<Macro>(first->c_cast<ast::Atom>().content());
            return MacroExpander::expand(
                macro, args_list(list.cbegin() + 1, list.cend()), _env);
        }
    }
    // Normal Expand Recursive : Expand expression tree
    ast::List result;
    for (decltype(list.size()) i = 0; i != list.size(); ++i) {
        auto& elem = list.craw()[i];
        result.push_back(expand_expression(elem));
    }
    return result;
}

} // namespace libdocscript::runtime