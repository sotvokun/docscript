#ifndef LIBDOCSCRIPT_AST_ATOM_H
#define LIBDOCSCRIPT_AST_ATOM_H
#include "libdocscript/ast/node.h"
#include "libdocscript/exception.h"
#include "libdocscript/libdocscript.h"
#include "libdocscript/misc.h"
#include <variant>

namespace docscript::libdocscript::ast {
enum class AtomType {
    Name,
    Integer,
    Decimal,
    Boolean,
    String
};

class Atom final : public Node {
  public:
    using string_type = std::string;
    using int_type = docscript::libdocscript::int_type;
    using decimal_type = docscript::libdocscript::decimal_type;
    using bool_type = bool;

    Atom(AtomType t, const string_type &s, Position p)
        : Node(NodeType::Atom, p), type_atom(t), _value(s) {}
    explicit Atom(int_type n, Position p)
        : Node(NodeType::Atom, p), type_atom(AtomType::Integer), _value(n) {}
    explicit Atom(decimal_type n, Position p)
        : Node(NodeType::Atom, p), type_atom(AtomType::Decimal), _value(n) {}
    explicit Atom(bool_type b, Position p)
        : Node(NodeType::Atom, p),
          type_atom(AtomType::Boolean),
          _value(b) {}
    ~Atom() override = default;

    template <typename T> inline T value() const {
        // THROW Unimplement Exception
        throw UnimplementException();
    }

    Position position;
    AtomType type_atom;

  private:
    std::variant<string_type, int_type, decimal_type, bool_type> _value;
};

template <> Atom::string_type Atom::value() const;
template <> Atom::int_type Atom::value() const;
template <> Atom::decimal_type Atom::value() const;
template <> Atom::bool_type Atom::value() const;
} // namespace docscript::libdocscript::ast
#endif