#ifndef DOCSIR_ENVIRONMENT_H
#define DOCSIR_ENVIRONMENT_H
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/value.h"
#include <string>
#include <unordered_map>

namespace docscript::docsir {
class Environment final {
  public:
    Environment() : _last_result(Value::List()) {}
    Environment(const std::string &name)
        : _name(name), _last_result(Value::List()) {}

    inline const std::string &name() const { return _name; }

    inline const Value &get_definition(const std::string &name) const {
        if (auto result = _deflist.find(name); result != _deflist.end()) {
            return result->second;
        } else {
            throw AccessUndefinedNameException(name);
        }
    }
    inline void set_definition(const std::string &name, const Value &val) {
        if (auto result = _deflist.find(name); result != _deflist.end()) {
            result->second = val;
        } else {
            _deflist.insert({name, val});
        }
    }
    inline void set_definition(const std::string &name,
                               BuiltInProcedure::function_type fn) {
        _deflist.insert({name, Value::BuiltInProcedure(fn)});
    }
    inline bool exist_definition(const std::string &name) const {
        return _deflist.find(name) != _deflist.end();
    }

    inline const Value &get_result() const { return _last_result; }
    inline void set_result(const Value &val) { _last_result = val; }

    inline Value to_value() const {
        Value result =
            Value::List({Value::Symbol("__INTERPRETER_DOCSIR"),
                         Value::String(_name), Value::List(), Value::List()});
        auto &deflist = result.cast<List>().value()[2];
        for (const auto &i : _deflist) {
            deflist.cast<List>().value().push_back(
                Value::List({Value::Symbol(i.first), i.second}));
        }
        return result;
    }

    inline static Environment from_value(const Value &val) {
        if (val.type() != ValueType::List) {
            throw TypeException("value must be a list");
        }
        const auto &list = val.cast<List>().value();
        if (list.size() != 4 || list[0].type() != ValueType::Symbol ||
            list[0].cast<Symbol>().value() != "__INTERPRETER_DOCSIR" ||
            list[1].type() != ValueType::String ||
            list[2].type() != ValueType::List) {
            throw TypeException(
                "Cannot convert an unstandardized list to environtment");
        }
        Environment result;
        result._name = list[1].cast<String>().value();
        for (const auto &i : list[2].cast<List>().value()) {
            if (i.type() != ValueType::List ||
                i.cast<List>().value().size() != 2) {
                throw TypeException(
                    "Cannot convert an unstandardized list to environtment");
            }
            const auto &pair = i.cast<List>().value();
            if (pair.front().type() != ValueType::Symbol) {
                throw TypeException(
                    "Cannot convert an unstandardized list to environtment");
            }
            result._deflist.insert(
                {pair.front().cast<Symbol>().value(), pair.back()});
        }
        return result;
    }

  private:
    std::string _name;
    Value _last_result;
    std::unordered_map<std::string, Value> _deflist;
};
} // namespace docscript::docsir

#endif