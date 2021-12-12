#ifndef LIBDOCSCRIPT_RUNTIME_MACRO_H
#define LIBDOCSCRIPT_RUNTIME_MACRO_H
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace libdocscript::runtime {

class Environment;
class MacroExpander;

class Macro final
{
    friend class MacroExpander;

public:
    using param_list = std::vector<std::string>;
    using arg_list = std::unordered_map<std::string, ast::Expression>;

    Macro(const param_list& params, ast::Expression expr);
    ~Macro() = default;

    const param_list& parameters() const;

private:
    param_list _params;
    ast::Expression _expression;
};
} // namespace libdocscript::runtime

#endif