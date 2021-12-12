#ifndef LIBDOCSCRIPT_RUNTIME_MACRO_EXPANDER
#define LIBDOCSCRIPT_RUNTIME_MACRO_EXPANDER
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/macro.h"
#include <unordered_map>
#include <vector>

namespace libdocscript::runtime {

class MacroExpander final
{
  public:
    using args_map = std::unordered_map<std::string, ast::Expression>;
    using args_list = std::vector<ast::Expression>;

    ///
    /// \brief Expand the macro once, the nested macro will not be expanded
    ///
    static ast::Expression expand_1(Macro& macro,
                                    const args_list& args,
                                    Environment& env);
    
    ///
    /// \brief Expand the macro recursively, all macro inside will be expanded.
    ///        IMPORTANT: THE SELF NESTED MACRO WILL MAKE STACK OVERFLOWED
    ///
    static ast::Expression expand(Macro& macro,
                                  const args_list& args,
                                  Environment& env);

  private:
    Environment& _env;
    Macro& _macro;
    bool _expand_recusrive;
    args_map &_args;

    MacroExpander(Macro& macro, args_map& args, Environment& env, bool rec_expand);

    ast::Expression expand_expression(const ast::Expression& expr);
    ast::Expression expand_atom(const ast::Atom& atom);
    ast::Expression expand_list(const ast::List& list);

    bool find_name(const std::string& name);

    static args_map create_map(const Macro::param_list &param, const args_list &args);
};

}

#endif