#include "docsir/value.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include <initializer_list>
#include <string>

namespace docscript::docsir {

// Template specialization

template <> const Symbol &Value::cast() const {
    return *dynamic_cast<docsir::Symbol *>(_data);
}

template <> const Number &Value::cast() const {
    return *dynamic_cast<docsir::Number *>(_data);
}

template <> const String &Value::cast() const {
    return *dynamic_cast<docsir::String *>(_data);
}

template <> const Boolean &Value::cast() const {
    return *dynamic_cast<docsir::Boolean *>(_data);
}

template <> const List &Value::cast() const {
    return *dynamic_cast<docsir::List *>(_data);
}

template <> const Procedure &Value::cast() const {
    return *dynamic_cast<docsir::Procedure *>(_data);
}

template <> const BuiltInProcedure &Value::cast() const {
    return *dynamic_cast<docsir::BuiltInProcedure *>(_data);
}

template <> const LambdaProcedure &Value::cast() const {
    return *dynamic_cast<docsir::LambdaProcedure *>(_data);
}

template <> Symbol &Value::cast() {
    return *dynamic_cast<docsir::Symbol *>(_data);
}

template <> Number &Value::cast() {
    return *dynamic_cast<docsir::Number *>(_data);
}

template <> String &Value::cast() {
    return *dynamic_cast<docsir::String *>(_data);
}

template <> Boolean &Value::cast() {
    return *dynamic_cast<docsir::Boolean *>(_data);
}

template <> List &Value::cast() {
    return *dynamic_cast<docsir::List *>(_data);
}

template <> Procedure &Value::cast() {
    return *dynamic_cast<docsir::Procedure *>(_data);
}

template <> BuiltInProcedure &Value::cast() {
    return *dynamic_cast<docsir::BuiltInProcedure *>(_data);
}

template <> LambdaProcedure &Value::cast() {
    return *dynamic_cast<docsir::LambdaProcedure *>(_data);
}

// Static constructor

Value Value::Symbol(const std::string &s) {
    return std::move(Value(new docsir::Symbol(s)));
}

Value Value::Number(ds_integer num) {
    return std::move(Value(new docsir::Number(num)));
}

Value Value::Number(ds_float num) {
    return std::move(Value(new docsir::Number(num)));
}

Value Value::String(const std::string &s) {
    return std::move(Value(new docsir::String(s)));
}

Value Value::Boolean(bool b) {
    return std::move(Value(new docsir::Boolean(b)));
}

Value Value::List() { return std::move(Value(new docsir::List())); }

Value Value::List(const std::initializer_list<Value> &list) {
    docsir::List::list_type result(list.begin(), list.end());
    return std::move(Value(new docsir::List(result)));
}

Value Value::List(const docsir::List::list_type &list) {
    return std::move(Value(new docsir::List(list)));
}

Value Value::BuiltInProcedure(BuiltInProcedure::function_type fn) {
    return std::move(Value(new docsir::BuiltInProcedure(fn)));
}

Value Value::LambdaProcedure(const LambdaProcedure::parameter_type &parm,
                             LambdaProcedure::body_type b) {
    return std::move(Value(new docsir::LambdaProcedure(parm, b)));
}

Value Value::LambdaProcedure(LambdaProcedure::body_type b) {
    return std::move(Value(new docsir::LambdaProcedure(b)));
}

// Constructor and Destructor

Value::~Value() {
    if (_data)
        delete _data;
}

DataType *Value::clone() const {
    DataType *result;
    switch (type()) {
    case ValueType::Number:
        if (cast<docsir::Number>().number_type() == docsir::Number::Integer) {
            result =
                new docsir::Number(cast<docsir::Number>().value<ds_integer>());
        } else {
            result =
                new docsir::Number(cast<docsir::Number>().value<ds_float>());
        }
        break;
    case ValueType::Boolean:
        result = new docsir::Boolean(cast<docsir::Boolean>().value());
        break;
    case ValueType::Symbol:
        result = new docsir::Symbol(cast<docsir::Symbol>().value());
        break;
    case ValueType::String:
        result = new docsir::String(cast<docsir::String>().value());
        break;
    case ValueType::List:
        result = new docsir::List(cast<docsir::List>().value());
        break;
    case ValueType::Procedure:
        if (cast<docsir::Procedure>().builtin()) {
            result = new docsir::BuiltInProcedure(
                cast<docsir::BuiltInProcedure>().value());
        } else {
            result = new docsir::LambdaProcedure(
                cast<docsir::LambdaProcedure>()._parameter,
                cast<docsir::LambdaProcedure>()._body);
        }
        break;
    }
    return result;
}

// Copy and move control member

Value::Value(const Value &val) { _data = val.clone(); }

Value::Value(Value &&val) : _data(val._data) { val._data = nullptr; }

Value &Value::operator=(const Value &val) {
    delete _data;
    _data = val.clone();
    return *this;
}

Value &Value::operator=(Value &&val) {
    if (_data && _data != val._data) {
        delete _data;
        _data = val._data;
    }
    val._data = nullptr;
    return *this;
}

} // namespace docscript::docsir