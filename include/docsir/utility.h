#ifndef DOCSIR_UTILITY_H
#define DOCSIR_UTILITY_H
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"
#include <iostream>
#include <string>

namespace docsir {
std::ostream& print(std::ostream& ostream,
                    const libdocscript::runtime::Value& val)
{
    switch (val.type()) {
    case libdocscript::runtime::DataType::Kind::String:
        ostream << "\"" << static_cast<std::string>(val) << "\"";
        break;

    case libdocscript::runtime::DataType::Kind::List: {
        ostream << "(";
        for (auto beg = val.c_cast<libdocscript::runtime::List>().cbegin();
             beg != val.c_cast<libdocscript::runtime::List>().cend(); ++beg)
        {
            print(ostream, *beg);
            if (beg != val.c_cast<libdocscript::runtime::List>().cend() - 1) {
                ostream << " ";
            }
        }
        ostream << ")";
    } break;

    default:
        ostream << static_cast<std::string>(val);
        break;
    }
    return ostream;
}
} // namespace docsir

#endif