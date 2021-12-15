#include "libdocscript/exception.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/macro.h"
#include <memory>
#include <string>
#include <utility>


namespace libdocscript::runtime {

// +---------------------------+
//         Constructor
// +---------------------------+

Environment::Environment() = default;

// +---------------------------+
//       Public Functions
// +---------------------------+

EnvironmentFindResult Environment::find(const std::string& name,
                                        bool current_only)
{
    auto iter = _value_dict.find(name);
    if (iter == _value_dict.end()) {
        if (!current_only && has_parent())
            return parent().find(name);
        else
            return EnvironmentFindResult::NotExist;
    }
    else {
        return is_macro(iter->second) ? EnvironmentFindResult::ExistMacro
                                      : EnvironmentFindResult::ExistValue;
    }
}

bool Environment::has_parent() const
{
    return _parent != nullptr;
}

Environment& Environment::parent()
{
    return *_parent;
}

Environment& Environment::global()
{
    Environment *e = this;
    while (e->has_parent()) {
        e = e->_parent;
    }
    return *e;
}

Environment Environment::derive()
{
    Environment env;
    env._parent = this;
    return env;
}

// +---------------------------+
//       Static Functions
// +---------------------------+

bool Environment::is_macro(const Environment::data_type& val)
{
    return val.index() == 0;
}

// +---------------------------+
//    Macro : Specialization
// +---------------------------+

template <>
bool Environment::find<Macro>(const std::string& name, bool current_only)
{
    auto iter = _value_dict.find(name);
    if (iter == _value_dict.end()) {
        if (!current_only && has_parent())
            return parent().find<Macro>(name);
        else
            return false;
    }
    else {
        return is_macro(iter->second);
    }
}

template <>
void Environment::set<Macro>(const std::string& name, Macro obj)
{
    _value_dict.insert_or_assign(name, obj);
}

template <>
Macro& Environment::get<Macro>(const std::string& name, bool current_only)
{
    if (!find<Macro>(name)) {
        throw UnboundedSymbol(name);
    }
    auto iter = _value_dict.find(name);
    if (iter != _value_dict.end()) {
        return std::get<Macro>(iter->second);
    }
    else if (!current_only && has_parent()) {
        return parent().get<Macro>(name, current_only);
    }
    else {
        throw InternalUnimplementException("Environment::get<Macro>()");
    }
}

// +---------------------------+
//    Value : Specialization
// +---------------------------+

template <>
bool Environment::find<Value>(const std::string& name, bool current_only)
{
    auto iter = _value_dict.find(name);
    if (iter == _value_dict.end()) {
        if (!current_only && has_parent())
            return parent().find<Value>(name);
        else
            return false;
    }
    else {
        return !is_macro(iter->second);
    }
}

template <>
void Environment::set<Value>(const std::string& name, Value obj)
{
    _value_dict.insert_or_assign(name, obj);
}

template <>
Value& Environment::get<Value>(const std::string& name, bool current_only)
{
    if (!find<Value>(name)) {
        throw UnboundedSymbol(name);
    }
    auto iter = _value_dict.find(name);
    if (iter != _value_dict.end()) {
        return std::get<Value>(iter->second);
    }
    else if (!current_only && has_parent()) {
        return parent().get<Value>(name, current_only);
    }
    else {
        throw InternalUnimplementException("Environment::get<Value>()");
    }
}
} // namespace libdocscript::runtime