#ifndef DOCSIR_DATATYPE_H
#define DOCSIR_DATATYPE_H
#include "docscript.h"
#include "libdocscript/ast.h"
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

namespace docscript::docsir {
enum class ValueType { Symbol, Number, String, Boolean, List, Procedure };

class Value;

class DataType {
  public:
    DataType(ValueType t) : _type(t) {}
    virtual ~DataType() = default;

    inline const ValueType &type() const { return _type; }
    inline virtual operator bool() const = 0;

  private:
    ValueType _type;
};

class Symbol final : public DataType {
  public:
    inline operator std::string() { return _value; }

    Symbol(const std::string &val) : DataType(ValueType::Symbol), _value(val) {}
    ~Symbol() override = default;

    inline const std::string &value() const { return _value; }
    inline operator bool() const override { return true; }

  private:
    std::string _value;
};

class Number final : public DataType {
  public:
    enum NumberType { Integer, Float };

    explicit Number(ds_integer i)
        : DataType(ValueType::Number), _num_type(Integer), _int_value(i),
          _float_value(i) {}
    explicit Number(ds_float f)
        : DataType(ValueType::Number), _num_type(Float), _int_value(f),
          _float_value(f) {}
    ~Number() override = default;

    inline NumberType number_type() const { return _num_type; }
    inline ds_integer value_int() const { return _int_value; }
    inline ds_float value_float() const { return _float_value; }

    template <typename T> inline const T &value() const {
        throw UnimplementException();
    }
    template <> inline const ds_integer &value() const { return _int_value; }
    template <> inline const ds_float &value() const { return _float_value; }

    inline operator bool() const override {
        return _float_value ? true : false;
    }

  private:
    Number(ds_integer i, ds_float f)
        : DataType(ValueType::Number), _int_value(i), _float_value(f) {}

    NumberType _num_type;
    ds_integer _int_value;
    ds_float _float_value;
};

class String final : public DataType {
  public:
    inline operator std::string() { return _value; }

    using string_type = std::string;
    String(const string_type &val) : DataType(ValueType::String), _value(val) {}
    ~String() override = default;

    inline const string_type &value() const { return _value; }
    inline operator bool() const override { return true; }

  private:
    string_type _value;
};

class Boolean final : public DataType {
  public:
    inline operator bool() { return _value; }

    Boolean(bool val) : DataType(ValueType::Boolean), _value(val) {}
    ~Boolean() override = default;

    inline const bool &value() const { return _value; }
    inline operator bool() const override { return _value; }

  private:
    bool _value;
};

class List;

class Procedure : public DataType {
  public:
    Procedure(bool builtin)
        : DataType(ValueType::Procedure), _builtin(builtin) {}
    ~Procedure() override = default;

    inline bool builtin() const { return _builtin; }
    inline operator bool() const override { return true; }

  private:
    bool _builtin = false;
};

class BuiltInProcedure final : public Procedure {
    friend class Value;

  public:
    using function_type = std::function<Value(std::vector<Value>)>;
    BuiltInProcedure(function_type val) : Procedure(true), _value(val) {}
    ~BuiltInProcedure() override = default;

    inline const function_type &value() const { return _value; }
    inline operator bool() const override { return true; }

  private:
    function_type _value;
};

class LambdaProcedure final : public Procedure {
    friend class Value;

  public:
    using parameter_type = std::vector<std::string>;
    using body_type = libdocscript::ast::SExpression;
    using env_type = std::unordered_map<std::string, Value>;

    LambdaProcedure(const parameter_type &param, body_type body)
        : Procedure(false), _parameter(param), _body(body) {}
    LambdaProcedure(body_type body) : Procedure(false), _body(body) {}
    ~LambdaProcedure() override = default;

    // there should has a function to invoke the procedure
    inline const parameter_type &parameter() const { return _parameter; }
    inline const body_type &body() const { return _body; }
    inline operator bool() const override { return true; }

    env_type env;

  private:
    parameter_type _parameter;
    body_type _body;
};

inline std::string datatype_stringify(ValueType type) {
    switch (type) {
    case ValueType::Symbol:
        return "symbol";
    case ValueType::Number:
        return "number";
    case ValueType::String:
        return "string";
    case ValueType::Boolean:
        return "boolean";
    case ValueType::List:
        return "list";
    case ValueType::Procedure:
        return "procedure";
    }
}
} // namespace docscript::docsir

#endif