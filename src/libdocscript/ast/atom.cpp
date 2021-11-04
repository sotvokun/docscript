#include "libdocscript/ast/atom.h"

namespace docscript::libdocscript::ast {
template <> Atom::string_type Atom::value() const {
    return std::get<string_type>(_value);
}

template <> Atom::int_type Atom::value() const {
    return std::get<int_type>(_value);
}

template <> Atom::decimal_type Atom::value() const {
    return std::get<decimal_type>(_value);
}

template <> Atom::bool_type Atom::value() const {
    return std::get<bool_type>(_value);
}
} // namespace docscript::libdocscript::ast