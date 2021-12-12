#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/exception.h"
#include "libdocscript/token.h"
#include <unordered_map>
#include <string>

namespace libdocscript::ast {

// +----------------------+
//       Constructor
// +----------------------+

Atom::Atom(AtomType t, const std::string& c)
  : ASTNode(ASTNodeType::Atom)
  , _type(t)
  , _content(c)
{}

Atom::Atom(Token t)
  : ASTNode(ASTNodeType::Atom)
{
    _content = t.content;
    if (t.type == TokenType::Identifier) {
        _type = special_type(_content);
    } else {
        _type = convert_type(t.type);
    }
}

// +----------------------+
//       Copy Control
// +----------------------+

Atom::Atom(const Atom& atom)
  : ASTNode(ASTNodeType::Atom)
  , _content(atom._content)
  , _type(atom._type)
{}

Atom::Atom(Atom&& atom)
  : ASTNode(ASTNodeType::Atom)
  , _content(atom._content)
  , _type(atom._type)
{}

// +----------------------+
//     Public Functions
// +----------------------+

AtomType
Atom::atom_type() const
{
    return _type;
}

std::string&
Atom::content()
{
    return _content;
}

const std::string&
Atom::content() const
{
    return _content;
}

Expression
Atom::ptr_clone() const
{
    return Expression(*this);
}

// +----------------------+
//     Private Functions
// +----------------------+

ASTNode*
Atom::rawptr_clone() const
{
    return new Atom(*this);
}

// +----------------------+
//     Static Functions
// +----------------------+

AtomType
Atom::convert_type(TokenType type)
{
    switch (type) {
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
Atom::special_type(const std::string& content)
{
    std::unordered_map<std::string, AtomType> dict{
        { "define", AtomType::Define },
        { "set!", AtomType::Set },
        { "lambda", AtomType::Lambda },
        { "and", AtomType::LogicAnd },
        { "or", AtomType::LogicOr },
        { "if", AtomType::IfElse },
        { "quote", AtomType::Quote },
        { "quasiquote", AtomType::Quasiquote },
        { "unquote", AtomType::Unquote },
        { "unquote-splicing", AtomType::UnquoteSplicing },
        { "define-macro", AtomType::DefineMacro }
    };

    auto iter = dict.find(content);
    if (iter == dict.end())
        return AtomType::Symbol;
    else
        return iter->second;
}

} // namespace libdocscript::ast