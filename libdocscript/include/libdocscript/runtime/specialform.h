#ifndef LIBDOCSCRIPT_RUNTIME_SPECIAL_FORM_H
#define LIBDOCSCRIPT_RUNTIME_SPECIAL_FORM_H
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/value.h"
#include <string>
#include <vector>

namespace libdocscript::runtime {
class SpecialForm
{
public:
    SpecialForm(const ast::List& list);
    virtual ~SpecialForm() = default;

    const std::string& form_name() const;
    virtual Value operator()(Environment& env) = 0;

    static const Value& check_value_validation(const Value& val);

private:
    std::string _form_name;
};
} // namespace libdocscript::runtime

namespace libdocscript::runtime::specialform {
class Define final : public SpecialForm
{
public:
    Define(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    std::string _name;
    ast::Expression _value_expr;
};

class Set final : public SpecialForm
{
public:
    Set(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    std::string _name;
    ast::Expression _value_expr;
};

class Lambda final : public SpecialForm
{
public:
    Lambda(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    std::vector<std::string> _name_list;
    ast::Expression _expr;
};

class LogicAnd final : public SpecialForm
{
public:
    LogicAnd(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    std::vector<ast::Expression> _exprs;
};

class LogicOr final : public SpecialForm
{
public:
    LogicOr(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    std::vector<ast::Expression> _exprs;
};

class IfElse final : public SpecialForm
{
public:
    IfElse(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    ast::Expression _cond;
    ast::Expression _then;
    ast::Expression _else;
};

class Quote final : public SpecialForm
{
public:
    Quote(const ast::List& list);

    Value operator()(Environment& env) override;

    static Value process(const ast::Expression& expr, Environment& env);
    static Value process_atom(const ast::Atom& expr, Environment& env);
    static Value process_list(const ast::List& list, Environment& env);

private:
    ast::Expression _expr;
};

class Unquote;
class UnquoteSplicing;

class Quasiquote final : public SpecialForm
{
public:
    Quasiquote(const ast::List& list);

    Value operator()(Environment& env) override;

    static Value process(const ast::Expression& expr, Environment& env,
                         int depth);
    static Value process_list(const ast::List& list, Environment& env,
                              int depth);

    static bool is_unquote(const ast::List& list);
    static bool is_splicing(const ast::List& list);
    static bool is_inner_splicing(const ast::List& list);

private:
    ast::Expression _expr;
};

class Unquote final : public SpecialForm
{
public:
    Unquote(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    ast::Expression _expr;
};

class UnquoteSplicing final : public SpecialForm
{
public:
    UnquoteSplicing(const ast::List& list);

    Value operator()(Environment& env) override;
    static Value expose_list(const List& val_list);

private:
    ast::Expression _expr;
};

class DefineMacro final : public SpecialForm
{
public:
    DefineMacro(const ast::List& list);

    Value operator()(Environment& env) override;

private:
    std::string _name;
    std::vector<std::string> _name_list;
    ast::Expression _expr;
};

} // namespace libdocscript::runtime::specialform

#endif