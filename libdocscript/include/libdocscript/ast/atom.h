#ifndef LIBDOCSCRIPT_AST_ATOM_H
#define LIBDOCSCRIPT_AST_ATOM_H
#include "libdocscript/ast/ast.h"
#include "libdocscript/token.h"
#include <string>
#include <unordered_map>
#include <utility>

namespace libdocscript::ast {

enum class AtomType
{
    Nil,
    Symbol,
    Boolean,
    String,
    Number,

    Define,
    Set,
    Lambda,
    LogicAnd,
    LogicOr,
    IfElse,
    Quote,
    Quasiquote,
    Unquote,
    UnquoteSplicing,
    DefineMacro
};

class Atom final : public ASTNode
{
    friend class List;

public:
    Atom(AtomType t, const std::string& c);
    Atom(Token t);
    Atom(const Atom&);
    Atom(Atom&&);
    ~Atom() override = default;

    AtomType atom_type() const;

    std::string& content();
    const std::string& content() const;

    Expression ptr_clone() const override;

    static AtomType convert_type(TokenType type);
    static AtomType special_type(const std::string& content);

private:
    std::string _content;
    AtomType _type;

    ASTNode* rawptr_clone() const override;
};

class AtomHelper final
{
public:
    AtomHelper() = default;
    ~AtomHelper() = default;

    AtomHelper(const AtomHelper&) = delete;
    AtomHelper(AtomHelper&&) = delete;
    AtomHelper& operator=(const AtomHelper&) = delete;
    AtomHelper& operator=(AtomHelper&&) = delete;

    AtomType get_type_by_token_type(TokenType tt) const;
    AtomType get_type_by_content(const std::string& content) const;
    Atom create_by_type(AtomType t, std::string content = "") const;
    Atom create_emptyline_symbol() const;

private:
    std::unordered_map<std::string, AtomType> _symbol_map = {
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
        { "define-macro", AtomType::DefineMacro },
    };
};
} // namespace libdocscript::ast

#endif