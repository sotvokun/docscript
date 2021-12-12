#include "libdocscript/runtime/specialform.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/exception.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

SpecialForm::SpecialForm(const ast::List& list)
  : _form_name(list.cbegin()->c_cast<ast::Atom>().content())
{}

// +--------------------+
//    Public Functions
// +--------------------+

const std::string&
SpecialForm::form_name() const
{
    return _form_name;
}

// +--------------------+
//    Static Functions
// +--------------------+

const Value &SpecialForm::check_value_validation(const Value &val)
{
    if(val.type() == DataType::Kind::Unspecific)
    {
        throw DefinitionException();
    }
    return val;
}
}