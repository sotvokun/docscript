#ifndef LIBDOCSCRIPT_RUNTIME_DATATYPE_H
#define LIBDOCSCRIPT_RUNTIME_DATATYPE_H
#include "libdocscript/exception.h"
#include <string>

namespace libdocscript::runtime {

class List;
class Procedure;
class Boolean;
class Value;

class DataType
{
    friend class Value;

public:
    enum class Kind
    {
        Unspecific,
        Nil,
        Error,
        Symbol,
        Boolean,
        Number,
        String,
        List,
        Procedure
    };

    DataType(DataType::Kind k);
    DataType(const DataType&) = default;

    DataType& operator=(const DataType&) = delete;
    DataType& operator=(DataType&&) = delete;

    virtual operator bool() const;
    virtual operator std::string() const;

    static std::string to_string(DataType::Kind k);

private:
    Kind _kind;

    virtual DataType* rawptr_clone() const;
};

class Unspecific final : public DataType
{
public:
    Unspecific() : DataType(DataType::Kind::Unspecific) {}

    operator bool() const override;
    virtual operator std::string() const override;

private:
    DataType* rawptr_clone() const override;
};

class Nil final : public DataType
{
public:
    Nil() : DataType(DataType::Kind::Nil) {}

    operator bool() const override;
    operator std::string() const override;

private:
    DataType* rawptr_clone() const override;
};

class Error final : public DataType
{
public:
    Error(const std::string& msg);

    const std::string& message() const;
    RuntimeError raise() const;

    operator bool() const override;
    operator std::string() const override;

private:
    std::string _msg;
    DataType* rawptr_clone() const override;
};

class Symbol final : public DataType
{
public:
    Symbol(const std::string& c);

    std::string& content();
    const std::string& const_content() const;

    operator bool() const override;
    operator std::string() const override;

private:
    std::string _content;
    DataType* rawptr_clone() const override;
};

class Boolean final : public DataType
{
public:
    Boolean(bool b);

    bool value() const;

    operator bool() const override;
    operator std::string() const override;

private:
    bool _value;
    DataType* rawptr_clone() const override;
};

class Number final : public DataType
{
public:
    using int_type = long long;
    using dec_type = long double;

    enum Type
    {
        Integer,
        Decimal
    };

    explicit Number(int_type num);
    explicit Number(int num);
    explicit Number(dec_type num);
    explicit Number(double num);

    int_type integer() const;
    dec_type decimal() const;
    Type type() const;

    void cast_to(Type t);

    operator bool() const override;
    operator std::string() const override;
    operator int_type() const;
    operator dec_type() const;

private:
    Type _type;
    union
    {
        int_type integer;
        dec_type decimal;
    } _value;

    DataType* rawptr_clone() const override;
};

class String final : public DataType
{
public:
    String(const std::string& str);

    std::string& value();
    const std::string& const_value() const;

    operator bool() const override;
    operator std::string() const override;

private:
    std::string _value;

    DataType* rawptr_clone() const override;
};

} // namespace libdocscript::runtime

#endif