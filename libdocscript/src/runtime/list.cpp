#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/value.h"
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace libdocscript::runtime {
// +--------------------+
//      Constructor
// +--------------------+

List::List()
  : DataType(DataType::Kind::List)
{}

List::List(const raw_type& raw_list)
  : DataType(DataType::Kind::List)
{
    _raw.insert(_raw.begin(), raw_list.begin(), raw_list.end());
}

List::List(raw_type::const_iterator beg, raw_type::const_iterator end)
  : DataType(DataType::Kind::List)
{
    _raw.insert(_raw.begin(), beg, end);
}

List::List(const List& list)
  : DataType(DataType::Kind::List)
{
    for (const auto& e : list._raw) {
        _raw.push_back(e);
    }
}

List::List(List&& list)
  : DataType(DataType::Kind::List)
{
    for (const auto& e : list._raw) {
        _raw.push_back(std::move(e));
    }
}

// +--------------------+
//    Public Functions
// +--------------------+

List::raw_type&
List::raw()
{
    return _raw;
}

const List::raw_type&
List::craw() const
{
    return _raw;
}

List::raw_type::size_type
List::size() const
{
    return _raw.size();
}

void
List::push_back(const Value& val)
{
    _raw.push_back(val);
}

void
List::push_back(Value&& val)
{
    _raw.push_back(std::move(val));
}

List::raw_type::iterator
List::begin()
{
    return _raw.begin();
}

List::raw_type::iterator
List::end()
{
    return _raw.end();
}

List::raw_type::const_iterator
List::cbegin() const
{
    return _raw.cbegin();
}

List::raw_type::const_iterator
List::cend() const
{
    return _raw.cend();
}

// +--------------------+
//   Private Functions
// +--------------------+

DataType*
List::rawptr_clone() const
{
    return new List(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

List::operator bool() const
{
    return _raw.size() != 0;
}

List::operator std::string() const
{
    std::ostringstream oss;
    oss << "(";
    for (auto beg = _raw.begin(); beg != _raw.end(); ++beg) {
        oss << static_cast<std::string>(*beg);
        if (beg != _raw.end() - 1) {
            oss << " ";
        }
    }
    oss << ")";
    return oss.str();
}

}