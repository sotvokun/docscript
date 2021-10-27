#ifndef DOCSIR_DATATYPE_LIST_H
#define DOCSIR_DATATYPE_LIST_H
#include "docsir/datatype.h"
#include "docsir/value.h"
#include <vector>

namespace docscript::docsir {
class List final : public DataType {
    friend class Value;

  public:
    using list_type = std::vector<Value>;
    List() : DataType(ValueType::List) {}
    List(const list_type &list) : DataType(ValueType::List), _value(list) {}
    ~List() override = default;

    inline const list_type &value() const { return _value; }
    inline list_type &value() { return _value; }

    inline operator bool() const override {
        return _value.size() ? true : false;
    }

  public:
    list_type _value;
};
} // namespace docscript::docsir

#endif