#ifndef LIBDOCSCRIPT_AST_H
#define LIBDOCSCRIPT_AST_H
#include "libdocscript/exception.h"
#include "libdocscript/misc.h"
#include "libdocscript/token.h"
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace docscript::libdocscript {
enum class ASTNodeType {
    Comment,
    Atom,
    TextString,
    TextLineEnd,
    Text,
    List,
    SExpressionList,
    SExpression,
};

class ASTNode {
  public:
    using ptr_type = std::shared_ptr<ASTNode>;

    ASTNode(ASTNodeType t, Position p) : type(t), position(p) {}
    virtual ~ASTNode() = default;

    virtual inline ptr_type share() { return ptr_type(new ASTNode(*this)); }

    ASTNodeType type;
    Position position;
};

class CommentAST : public ASTNode {
  public:
    CommentAST(const std::string &s, Position p)
        : ASTNode(ASTNodeType::Comment, p), value(s) {}
    ~CommentAST() override = default;

    inline ptr_type share() override { return ptr_type(new CommentAST(*this)); }

    std::string value;
};

enum class AtomType {
    Identifier,
    Symbol,
    IntNumber,
    FloatNumber,
    Boolean,
    String
};
class AtomAST : public ASTNode {
  public:
    explicit AtomAST(ds_integer n, Position p)
        : ASTNode(ASTNodeType::Atom, p), atom_type(AtomType::IntNumber),
          _value(n) {}
    explicit AtomAST(ds_float n, Position p)
        : ASTNode(ASTNodeType::Atom, p), atom_type(AtomType::FloatNumber),
          _value(n) {}
    explicit AtomAST(bool b, Position p)
        : ASTNode(ASTNodeType::Atom, p), atom_type(AtomType::Boolean),
          _value(b) {}
    ~AtomAST() override = default;

    inline ptr_type share() override { return ptr_type(new AtomAST(*this)); }

    template <typename T> inline T value() const {
        // THROW Implement Exception
        throw UnimplementException();
    }

    template <> inline ds_float value() const {
        return std::get<ds_float>(_value);
    }
    template <> inline ds_integer value() const {
        return std::get<ds_integer>(_value);
    }
    template <> inline bool value() const { return std::get<bool>(_value); }
    template <> inline std::string value() const {
        return std::get<std::string>(_value);
    }

    inline static AtomAST Identifier(const std::string &s, Position p) {
        return AtomAST(s, p, AtomType::Identifier);
    }
    inline static AtomAST String(const std::string &s, Position p) {
        return AtomAST(s, p, AtomType::String);
    }
    inline static AtomAST Symbol(const std::string &s, Position p) {
        return AtomAST(s, p, AtomType::Symbol);
    }
    inline static AtomAST Integer(ds_integer n, Position p) {
        return AtomAST(n, p);
    }
    inline static AtomAST Float(ds_float n, Position p) {
        return AtomAST(n, p);
    }
    inline static AtomAST Boolean(bool b, Position p) { return AtomAST(b, p); }

    AtomType atom_type;

  private:
    AtomAST(const std::string &s, Position p, AtomType t)
        : ASTNode(ASTNodeType::Atom, p), atom_type(t), _value(s) {}

    std::variant<ds_float, ds_integer, bool, std::string> _value;
};

class TextStringAST : public ASTNode {
  public:
    TextStringAST(const std::string &s, Position p)
        : ASTNode(ASTNodeType::TextString, p), value(s) {}
    ~TextStringAST() override = default;

    inline ptr_type share() override {
        return ptr_type(new TextStringAST(*this));
    }

    std::string value;
};

class TextLineEndAST : public ASTNode {
  public:
    TextLineEndAST(Position p) : ASTNode(ASTNodeType::TextLineEnd, p) {}
    ~TextLineEndAST() override = default;

    inline ptr_type share() override {
        return ptr_type(new TextLineEndAST(*this));
    }
};

class TextAST : public ASTNode {
  public:
    using value_type = std::vector<std::shared_ptr<ASTNode>>;

    TextAST(Position p) : ASTNode(ASTNodeType::Text, p) {}
    ~TextAST() override = default;

    inline ptr_type share() override { return ptr_type(new TextAST(*this)); }

    value_type value;
};

class ListAST : public ASTNode {
  public:
    using value_type = std::vector<std::shared_ptr<ASTNode>>;

    ListAST(Position p) : ASTNode(ASTNodeType::List, p) {}
    ~ListAST() override = default;

    inline ptr_type share() override { return ptr_type(new ListAST(*this)); }

    value_type value;
};

class SExpressionListAST : public ASTNode {
  public:
    using value_type = std::vector<std::shared_ptr<ASTNode>>;

    SExpressionListAST(Position p) : ASTNode(ASTNodeType::SExpressionList, p) {}
    ~SExpressionListAST() override = default;

    inline ptr_type share() override {
        return ptr_type(new SExpressionListAST(*this));
    }

    value_type value;
};

class SExpressionAST;
class SExpressionAST : public ASTNode {
  public:
    using value_type = std::shared_ptr<ASTNode>;

    SExpressionAST(CommentAST comment)
        : ASTNode(ASTNodeType::SExpression, comment.position),
          sexpr_type(ASTNodeType::Comment), value(comment.share()) {}
    SExpressionAST(AtomAST atom)
        : ASTNode(ASTNodeType::SExpression, atom.position),
          sexpr_type(ASTNodeType::Atom), value(atom.share()) {}
    SExpressionAST(TextAST text)
        : ASTNode(ASTNodeType::SExpression, text.position),
          sexpr_type(ASTNodeType::Text), value(text.share()) {}
    SExpressionAST(ListAST list)
        : ASTNode(ASTNodeType::SExpression, list.position),
          sexpr_type(ASTNodeType::List), value(list.share()) {}
    SExpressionAST(SExpressionListAST sexprlist)
        : ASTNode(ASTNodeType::SExpression, sexprlist.position),
          sexpr_type(ASTNodeType::SExpressionList), value(sexprlist.share()) {}
    ~SExpressionAST() override = default;

    inline ptr_type share() override {
        return ptr_type(new SExpressionAST(*this));
    }

    value_type value;
    ASTNodeType sexpr_type;
};
} // namespace docscript::libdocscript

#endif