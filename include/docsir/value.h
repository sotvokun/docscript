#ifndef DOCSIR_VALUE_H
#define DOCSIR_VALUE_H
#include "docscript.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include <initializer_list>
#include <string>

namespace docscript::docsir {
class Value final {
  public:
    Value(DataType *p) : _data(p) {}
    ~Value();

    Value(const Value &);
    Value(Value &&);

    Value &operator=(const Value &);
    Value &operator=(Value &&);

    inline ValueType type() const { return _data->type(); }

    template <typename T> inline const T &cast() const {
        throw UnimplementException();
    }
    template <typename T> inline T &cast() { throw UnimplementException(); }
    static Value Symbol(const std::string &s);
    static Value Number(ds_integer num);
    static Value Number(ds_float num);
    static Value String(const std::string &s);
    static Value Boolean(bool b);
    static Value List();
    static Value List(const std::initializer_list<Value> &);
    static Value List(const std::vector<Value> &);
    static Value BuiltInProcedure(docsir::BuiltInProcedure::function_type);
    static Value
    LambdaProcedure(const docsir::LambdaProcedure::parameter_type &,
                    docsir::LambdaProcedure::body_type);
    static Value LambdaProcedure(docsir::LambdaProcedure::body_type);

    inline operator bool() const { return _data->operator bool(); }

  private:
    DataType *clone() const;

    DataType *_data;
};

template <> const Symbol &Value::cast() const;
template <> const Number &Value::cast() const;
template <> const String &Value::cast() const;
template <> const Boolean &Value::cast() const;
template <> const List &Value::cast() const;
template <> const Procedure &Value::cast() const;
template <> const BuiltInProcedure &Value::cast() const;
template <> const LambdaProcedure &Value::cast() const;

template <> Symbol &Value::cast();
template <> Number &Value::cast();
template <> String &Value::cast();
template <> Boolean &Value::cast();
template <> List &Value::cast();
template <> Procedure &Value::cast();
template <> BuiltInProcedure &Value::cast();
template <> LambdaProcedure &Value::cast();

} // namespace docscript::docsir

#endif