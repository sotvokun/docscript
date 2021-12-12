#ifndef LIBDOCSCRIPT_INTERPRETER_LIST_H
#define LIBDOCSCRIPT_INTERPRETER_LIST_H
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/value.h"
#include "libdocscript/runtime/list.h"
#include <vector>

namespace libdocscript::runtime {
class List final : public DataType
{
  public:
    using raw_type = std::vector<Value>;

    List();

    List(const raw_type &);
    List(raw_type::const_iterator beg, raw_type::const_iterator end);

    List(const List &);
    List(List &&);

    raw_type &raw();
    const raw_type &craw() const;

    raw_type::size_type size() const;

    void push_back(const Value &val);
    void push_back(Value &&val);

    raw_type::iterator begin();
    raw_type::iterator end();
    raw_type::const_iterator cbegin() const;
    raw_type::const_iterator cend() const;

    operator bool() const override;
    operator std::string() const override;

  private:
    std::vector<Value> _raw;

    DataType *rawptr_clone() const override;
};
}

#endif