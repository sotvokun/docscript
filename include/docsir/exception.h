#ifndef DOCSIR_EXCEPTION_H
#define DOCSIR_EXCEPTION_H
#include "docscript.h"
#include "docsir/datatype.h"
#include <exception>
#include <sstream>
#include <string>

namespace docscript::docsir {
class UnimplementException : public std::exception {};

class BasicException : public std::exception {
  public:
    BasicException(const std::string &name, const std::string &msg)
        : name(name), message(msg) {}
    ~BasicException() override = default;

    const char *what() const override { return (name + ": " + message).c_str(); }
    inline std::string msg() const { return (name + ": " + message); }

    const std::string name;
    const std::string message;
};

class InterpreterException : public BasicException {
  public:
    using BasicException::BasicException;
    InterpreterException(const std::string &msg) : BasicException(_name, msg) {}
    ~InterpreterException() override = default;

  private:
    static constexpr char *_name = "InterpreterException";
};

class ArgumentException : public InterpreterException {
  public:
    ArgumentException(const std::string &msg)
        : InterpreterException(_name, msg) {}
    ~ArgumentException() override = default;

    inline static ArgumentException unexpected(size_t expect, size_t given,
                                               bool more = false) {
        std::ostringstream oss;
        oss << "Unexptected number of arguments be given, expect " << expect
            << (more ? " or more" : "") << ", given " << given;
        return ArgumentException(oss.str());
    }

  private:
    static constexpr char *_name = "ArgumentException";
};

class TypeException : public InterpreterException {
  public:
    TypeException(const std::string &msg) : InterpreterException(_name, msg) {}
    ~TypeException() override = default;

    inline static TypeException unexpected(ValueType expect, ValueType given) {
        std::ostringstream oss;
        oss << "Unexpected type of argument be given, expect "
            << datatype_stringify(expect) << ", given "
            << datatype_stringify(given);
        return TypeException(oss.str());
    }

  private:
    static constexpr char *_name = "TypeException";
};

class DivisionByZeroException : public InterpreterException {
  public:
    DivisionByZeroException()
        : InterpreterException(_name, "Division by zero") {}
    ~DivisionByZeroException() override = default;

  private:
    static constexpr char *_name = "DivisionByZeroException";
};

class AccessUndefinedNameException : public InterpreterException {
  public:
    AccessUndefinedNameException(const std::string &name)
        : InterpreterException(_name, name + ": undefined") {}
    ~AccessUndefinedNameException() override = default;

  private:
    static constexpr char *_name = "AccessUndefinedNameException";
};

class BadSyntaxException : public InterpreterException {
  public:
    BadSyntaxException(const std::string &keyword)
        : InterpreterException(_name, keyword + ": bad syntax") {}
    ~BadSyntaxException() override = default;

  private:
    static constexpr char *_name = "BadSyntaxException";
};

class DefineExistedNameException : public InterpreterException {
  public:
    DefineExistedNameException(const std::string &name)
        : InterpreterException(_name, name + ": existed") {}
    ~DefineExistedNameException() override = default;

  private:
    static constexpr char *_name = "DefineExistedNameException";
};

class CustomErrorException : public InterpreterException {
  public:
    CustomErrorException(const std::string &msg)
        : InterpreterException(_name, msg) {}
    ~CustomErrorException() override = default;

  private:
    static constexpr char *_name = "CustomError";
};
} // namespace docscript::docsir

#endif