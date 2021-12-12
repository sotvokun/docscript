#ifndef LIBDOCSCRIPT_INTERPRETER_H
#define LIBDOCSCRIPT_INTERPRETER_H
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"
#include <string>
#include <vector>

namespace libdocscript {

class Interpreter final
{
  public:
    Interpreter(runtime::Environment& env);

    runtime::Value eval(const ast::Expression& expr);

    runtime::Value eval_atom(const ast::Atom& atom);
    runtime::Nil eval_nil(const ast::Atom& atom);
    runtime::Value eval_symbol(const ast::Atom& atom);
    runtime::Boolean eval_boolean(const ast::Atom& atom);
    runtime::String eval_string(const ast::Atom& atom);
    runtime::Number eval_number(const ast::Atom& atom);

    runtime::Value eval_list(const ast::List& list);
    runtime::List eval_textlist(const ast::List& list);
    runtime::Value eval_procedure_calling(const ast::List& list);
    runtime::Value eval_special_form(const ast::List& list);

    // +--------------------+
    //         Macro
    // +--------------------+

    ast::Expression expand_macro(const ast::List& list);

    // +--------------------+
    //    Static Functions
    // +--------------------+
    static std::vector<std::string> get_name_list(
      const ast::Expression& node,
      const std::string& parent_form_name);

  private:
    runtime::Environment& _env;
};
}

#endif