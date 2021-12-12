#include "libdocscript/ast/atom.h"
#include "libdocscript/exception.h"
#include "libdocscript/token.h"
#include <unordered_map>
#include <algorithm>

namespace libdocscript::ast {

AtomType
AtomHelper::get_type_by_token_type(TokenType tt) const
{
    switch (tt) {
        case TokenType::Boolean:
            return AtomType::Boolean;
        case TokenType::Identifier:
            return AtomType::Symbol;
        case TokenType::String:
            return AtomType::String;
        case TokenType::Number:
            return AtomType::Number;
        default:
            throw InternalUnimplementException("Atom::convert_type()");
    }
}

AtomType
AtomHelper::get_type_by_content(const std::string& content) const
{
    auto iter =
      std::find_if(_symbol_map.begin(),
                   _symbol_map.end(),
                   [&](const auto& pair) { return pair.first == content; });
    if (iter == _symbol_map.end()) {
        return AtomType::Symbol;
    } else {
        return iter->second;
    }
}

Atom
AtomHelper::create_by_type(AtomType t, std::string content) const
{
    auto iter =
      std::find_if(_symbol_map.begin(),
                   _symbol_map.end(),
                   [&](const auto& pair) { return pair.second == t; });
    if (iter == _symbol_map.end()) {
        return Atom(t, content);
    } else {
        return Atom(t, iter->first);
    }
}

Atom
AtomHelper::create_emptyline_symbol() const
{
    return Atom(AtomType::Symbol, "empty line");
}
}