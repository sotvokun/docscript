#ifndef LIBDOCSCRIPT_RUNTIME_VALUE_H
#define LIBDOCSCRIPT_RUNTIME_VALUE_H
#include "libdocscript/exception.h"
#include "libdocscript/runtime/datatype.h"
#include <string>
#include <typeinfo>


namespace libdocscript::runtime {
class Value final
{
public:
    Value(const DataType& dt);

    Value(const Value&);
    Value(Value&&);
    Value& operator=(const Value&);
    Value& operator=(Value&&);

    ~Value();

    DataType::Kind type() const;

    template <typename T>
    T& cast()
    {
        try {
            return dynamic_cast<T&>(*_ptr);
        }
        catch (const std::bad_cast&) {
            throw InternalUnimplementException("Value::cast<T>()");
        }
    }

    template <typename T>
    const T& c_cast() const
    {
        return const_cast<Value&>(*this).cast<T>();
    }

    operator bool() const;
    operator std::string() const;
    bool operator==(const Value& rhs);
    bool operator!=(const Value& rhs);

private:
    DataType* _ptr;
};
} // namespace libdocscript::runtime

#endif