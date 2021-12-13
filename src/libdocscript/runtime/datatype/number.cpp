#include "libdocscript/runtime/datatype.h"
#include <limits>
#include <sstream>
#include <string>


namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

Number::Number(int_type num) : DataType(DataType::Kind::Number), _type(Integer)
{
    _value.integer = num;
}

Number::Number(int num) : DataType(DataType::Kind::Number), _type(Integer)
{
    _value.integer = num;
}

Number::Number(dec_type num) : DataType(DataType::Kind::Number), _type(Decimal)
{
    _value.decimal = num;
}

Number::Number(double num) : DataType(DataType::Kind::Number), _type(Decimal)
{
    _value.decimal = num;
}

// +--------------------+
//    Public Functions
// +--------------------+

Number::int_type Number::integer() const
{
    if (_type != Integer) {
        return static_cast<int_type>(_value.decimal);
    }
    return _value.integer;
}

Number::dec_type Number::decimal() const
{
    if (_type != Decimal) {
        return static_cast<dec_type>(_value.integer);
    }
    return _value.decimal;
}

void Number::cast_to(Type t)
{
    if (t == Integer && _type == Decimal) {
        _value.integer = _value.decimal;
    }
    else if (t == Decimal && _type == Integer) {
        _value.decimal = _value.integer;
    }
    _type = t;
    return;
}

Number::Type Number::type() const
{
    return _type;
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType* Number::rawptr_clone() const
{
    return new Number(*this);
}

// +--------------------+
//     Type Conversion
// +--------------------+

Number::operator bool() const
{
    if (_type == Integer) {
        return _value.integer != 0;
    }
    else {
        auto value = _value.decimal;
        if (value == std::numeric_limits<dec_type>().signaling_NaN() ||
            value == std::numeric_limits<dec_type>().signaling_NaN() * -1 ||
            value == 0)
        {
            return true;
        }
        else {
            return false;
        }
    }
}

Number::operator std::string() const
{
    std::ostringstream oss;
    if (_type == Integer) {
        oss << _value.integer;
        return oss.str();
    }
    else {
        auto value = _value.decimal;
        if (value == std::numeric_limits<dec_type>().signaling_NaN())
            oss << "+nan";
        else if (value == std::numeric_limits<dec_type>().signaling_NaN() * -1)
            oss << "-nan";
        else if (value == std::numeric_limits<dec_type>().infinity())
            oss << "+inf";
        else if (value == std::numeric_limits<dec_type>().infinity() * -1)
            oss << "-inf";
        else
            oss << value;
        return oss.str();
    }
}

Number::operator libdocscript::runtime::Number::int_type() const
{
    return integer();
}

Number::operator libdocscript::runtime::Number::dec_type() const
{
    return decimal();
}

} // namespace libdocscript::runtime