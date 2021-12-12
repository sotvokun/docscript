#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/value.h"
#include <string>


namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

Value::Value(const DataType& dt) : _ptr(dt.rawptr_clone()) {}

// +--------------------+
//      Copy Control
// +--------------------+

Value::Value(const Value& val) : _ptr(val._ptr->rawptr_clone()) {}

Value::Value(Value&& val) : _ptr(val._ptr)
{
    val._ptr = nullptr;
}

// +--------------------+
//      Destructor
// +--------------------+

Value::~Value()
{
    if (_ptr) {
        delete _ptr;
    }
}

// +--------------------+
//   Operator Overload
// +--------------------+

Value& Value::operator=(const Value& rhs)
{
    if (_ptr && _ptr != rhs._ptr) {
        delete _ptr;
    }
    _ptr = rhs._ptr->rawptr_clone();
    return *this;
}

Value& Value::operator=(Value&& rhs)
{
    if (_ptr) {
        delete _ptr;
    }
    _ptr = rhs._ptr;
    rhs._ptr = nullptr;
    return *this;
}

bool Value::operator==(const Value& rhs)
{
    if (type() != rhs.type()) return false;

    switch (type()) {
    case DataType::Kind::Unspecific:
    case DataType::Kind::Nil:
    case DataType::Kind::Procedure:
        return true;

    case DataType::Kind::Error:
        return c_cast<Error>().message() == rhs.c_cast<Error>().message();

    case DataType::Kind::Symbol:
        return c_cast<Symbol>().const_content() ==
               rhs.c_cast<Symbol>().const_content();

    case DataType::Kind::Boolean:
        return c_cast<Boolean>().value() == rhs.c_cast<Boolean>().value();

    case DataType::Kind::String:
        return c_cast<String>().const_value() ==
               rhs.c_cast<String>().const_value();

    case DataType::Kind::Number:
        return c_cast<Number>().decimal() == rhs.c_cast<Number>().decimal();

    case DataType::Kind::List: {
        const auto& self = c_cast<List>();
        const auto& rhs_list = rhs.c_cast<List>();
        for (decltype(self.size()) i = 0; i != self.size(); ++i) {
            const auto& self_elem = self.craw()[i];
            const auto& rhs_elem = rhs_list.craw()[i];
            if (self_elem != rhs_elem) {
                return false;
            }
        }
        return true;
    }

    default:
        throw InternalUnimplementException("Value::operator==()");
    }
}

bool Value::operator!=(const Value& rhs)
{
    return !(this->operator==(rhs));
}

// +--------------------+
//   Public Functions
// +--------------------+

DataType::Kind Value::type() const
{
    return _ptr->_kind;
}

// +--------------------+
//    Type Conversion
// +--------------------+

Value::operator bool() const
{
    return static_cast<bool>(*_ptr);
}

Value::operator std::string() const
{
    return static_cast<std::string>(*_ptr);
}

} // namespace libdocscript::runtime