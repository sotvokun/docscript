#ifndef LIBDOCSCRIPT_RUNTIME_ENVIRONMENT_H
#define LIBDOCSCRIPT_RUNTIME_ENVIRONMENT_H
#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/exception.h"
#include "libdocscript/runtime/macro.h"
#include "libdocscript/runtime/value.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace libdocscript::runtime {

enum class EnvironmentFindResult
{
    NotExist,
    ExistValue,
    ExistMacro
};

class Environment final
{
public:
    using data_type = std::variant<Macro, Value>;
    using dict_type = std::unordered_map<std::string, data_type>;

    Environment();

    template <typename T>
    inline bool find(const std::string& name, bool current_only = false)
    {
        throw InternalUnimplementException("Environment::find<T>()");
    }

    template <typename T>
    inline void set(const std::string& name, T obj)
    {
        throw InternalUnimplementException("Environment::set<T>()");
    }

    template <typename T>
    inline T& get(const std::string& name, bool current_only = false)
    {
        throw InternalUnimplementException("Environment::get<T>()");
    }

    EnvironmentFindResult find(const std::string& name,
                               bool current_only = false);

    bool has_parent() const;
    Environment& parent();
    Environment& global();

    Environment derive();

private:
    std::shared_ptr<Environment> _parent;

    dict_type _value_dict;

    static bool is_macro(const data_type&);
};

Environment& initialize_environment(Environment&);

template <>
bool Environment::find<Macro>(const std::string& name, bool current_only);

template <>
void Environment::set<Macro>(const std::string& name, Macro obj);

template <>
Macro& Environment::get<Macro>(const std::string& name, bool current_only);

template <>
bool Environment::find<Value>(const std::string& name, bool current_only);

template <>
void Environment::set<Value>(const std::string& name, Value val);

template <>
Value& Environment::get<Value>(const std::string& name, bool current_only);

} // namespace libdocscript::runtime

#endif