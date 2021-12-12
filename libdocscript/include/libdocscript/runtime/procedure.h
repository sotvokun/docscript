#ifndef LIBDOCSCRIPT_RUNTIME_PROCEDURE_H
#define LIBDOCSCRIPT_RUNTIME_PROCEDURE_H
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/ast/expression.h"
#include <vector>
#include <string>
#include <functional>

namespace libdocscript::runtime {
class Procedure : public DataType
{
  public:
    using args_list = std::vector<Value>;

    enum Type
    {
        Lambda,
        BuiltIn
    };

    Procedure(Type t);

    virtual Value invoke(const args_list &args, Environment &env) const;

    operator bool() const final;
    operator std::string() const override;
  
  private:
    Type _type;

    DataType *rawptr_clone() const override;
};

class LambdaProcedure final : public Procedure
{
  public:
    using parm_list = std::vector<std::string>;
    using func_body = ast::Expression;

    LambdaProcedure(const parm_list &parameters, func_body body);

    Value invoke(const args_list &args, Environment &env) const override;

    operator std::string() const override;
    
  private:
    parm_list _parameters;
    func_body _expression;

    DataType *rawptr_clone() const override;
};

class BuiltInProcedure final : public Procedure
{
  public:
    using inner_type = std::function<Value(const args_list &, Environment&)>;

    BuiltInProcedure(inner_type func);

    Value invoke(const args_list &args, Environment &env) const override;

    operator std::string() const override;

  private:
    inner_type _func;

    DataType *rawptr_clone() const override;
};

}

#endif