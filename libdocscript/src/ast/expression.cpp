#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/list.h"

namespace libdocscript::ast {

// +-------------------+
//      Constructor
// +-------------------+

Expression::Expression(ASTNode* ptr)
  : _ptr(ptr->rawptr_clone())
{}

Expression::Expression(const ASTNode& node)
  : _ptr(node.rawptr_clone())
{}

// +-------------------+
//      Copy Control
// +-------------------+

Expression::Expression(const Expression& expr)
  : _ptr(expr._ptr->rawptr_clone())
{}

Expression::Expression(Expression&& expr)
  : _ptr(expr._ptr)
{
    expr._ptr = nullptr;
}

Expression&
Expression::operator=(const Expression& rhs)
{
    // TODO check whether the procedure is correct?
    if (_ptr != nullptr && _ptr != rhs._ptr) {
        delete _ptr;
    }
    _ptr = rhs._ptr->rawptr_clone();
    return *this;
}

Expression&
Expression::operator=(Expression&& rhs)
{
    // TODO check whether the procedure is correct?
    if (_ptr != nullptr) {
        delete _ptr;
    }
    _ptr = rhs._ptr;
    rhs._ptr = nullptr;
    return *this;
}

// +-------------------+
//      Destructor
// +-------------------+

Expression::~Expression()
{
    if (_ptr != nullptr) {
        delete _ptr;
    }
}

// +-------------------+
//   Operator Overload
// +-------------------+

ASTNode&
Expression::operator*()
{
    return *_ptr;
}

const ASTNode&
Expression::operator*() const
{
    return *_ptr;
}

// +-------------------+
//   Public Functions
// +-------------------+

ASTNodeType
Expression::type() const
{
    return _ptr->type();
}

// +-----------------------+
//   Public Specialization
// +-----------------------+

template<>
Atom&
Expression::cast<Atom>()
{
    return dynamic_cast<Atom&>(*_ptr);
}

template<>
List&
Expression::cast<List>()
{
    return dynamic_cast<List&>(*_ptr);
}

template<>
const Atom&
Expression::c_cast<Atom>() const
{
    return dynamic_cast<const Atom&>(*_ptr);
}

template<>
const List&
Expression::c_cast<List>() const
{
    return dynamic_cast<const List&>(*_ptr);
}

} // namespace libdocscript::ast