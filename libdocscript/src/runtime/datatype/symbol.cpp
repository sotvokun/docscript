#include "libdocscript/runtime/datatype.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

Symbol::Symbol(const std::string& c)
  : DataType(DataType::Kind::Symbol)
  , _content(c)
{}

// +--------------------+
//    Public Functions
// +--------------------+

std::string &Symbol::content() {
    return _content;
}

const std::string &Symbol::const_content() const {
    return _content;
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType *Symbol::rawptr_clone() const {
    return new Symbol(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

Symbol::operator bool() const {
    return true;
}

Symbol::operator std::string() const {
    return _content;
}

}