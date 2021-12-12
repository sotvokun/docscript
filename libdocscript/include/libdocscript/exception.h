#ifndef LIBDOCSCRIPT_EXCEPTION_H
#define LIBDOCSCRIPT_EXCEPTION_H
#include "libdocscript/utility/position.h"
#include <exception>
#include <sstream>
#include <string>

namespace libdocscript {

enum class ExceptionType
{
    Warning,
    Error,
};

class Exception : public std::exception
{
  public:
    Exception(ExceptionType t, const std::string& msg)
      : type(t)
      , message(msg)
    {}

    inline const ExceptionType& get_type() const { return type; }
    inline const std::string& get_message() const { return message; }

    inline const char* what() const override
    {
        if (_content.size() == 0) {
            const_cast<Exception&>(*this)._content =
              std::string(type == ExceptionType::Warning ? "warning: "
                                                         : "error: ") +
              message;
        }
        return _content.c_str();
    }

  protected:
    ExceptionType type;
    std::string message;

  private:
    std::string _content;
};

class InputException : public Exception
{
  public:
    InputException(ExceptionType t, const std::string& msg, Position pos)
      : Exception(t, msg)
      , position(pos)
    {}

    inline const char* what() const final
    {
        if (_content.size() == 0) {
            std::ostringstream oss;
            oss << position.to_string() << ":";
            if (type == ExceptionType::Warning)
                oss << "warning: ";
            else
                oss << "error: ";
            oss << message;
            const_cast<InputException&>(*this)._content = oss.str();
        }
        return _content.c_str();
    }

  protected:
    Position position;

  private:
    std::string _content;
};

class InvalidIdentifier final : public InputException
{
  public:
    InvalidIdentifier(char ch, const Position& pos)
      : InputException(ExceptionType::Error, format_message(ch), pos)
    {}

  private:
    inline static std::string format_message(char ch)
    {
        std::ostringstream oss;
        oss << "invalid identifier with character ";
        oss << "`" << ch << "'";
        oss << " inside";
        return oss.str();
    }
};

class InvalidEscapeSequence final : public InputException
{
  public:
    InvalidEscapeSequence(char escape_ch, const Position& pos)
      : InputException(ExceptionType::Error, format_message(escape_ch), pos)
    {}

  private:
    inline static std::string format_message(char ch)
    {
        std::ostringstream oss;
        oss << "invalid escape sequence: ";
        oss << "`\\" << ch << "'";
        return oss.str();
    }
};

class UnfinishedInput : public InputException
{
  public:
    UnfinishedInput(const std::string& msg, const Position& pos)
      : InputException(ExceptionType::Error, msg, pos)
    {}
};

class UnclosedList final : public Exception
{
  public:
    UnclosedList()
      : Exception(ExceptionType::Error, "unclosed list")
    {}

    UnclosedList(const std::string& bracket)
      : Exception(ExceptionType::Error, "unclosed list, except: " + bracket)
    {}
};

class IllegalKeyword final : public Exception
{
  public:
    IllegalKeyword()
      : Exception(ExceptionType::Error, "illegal keyword")
    {}

    IllegalKeyword(const std::string& undefined_str)
      : Exception(ExceptionType::Error,
                  "illegal keyword, undefined: " +
                  undefined_str)
    {}
};

class InternalUnimplementException final : public Exception
{
  public:
    InternalUnimplementException(const std::string& msg)
      : Exception(ExceptionType::Error, "internal unimplement: " + msg)
    {}
};

class UnboundedSymbol final : public Exception
{
  public:
    UnboundedSymbol(const std::string& sym_name)
      : Exception(ExceptionType::Error, "unbounded symbol: " + sym_name)
    {}
};

class RuntimeError final : public Exception
{
  public:
    RuntimeError(const std::string& msg)
      : Exception(ExceptionType::Error, msg)
    {}
};

class InternalParsingException final : public Exception
{
  public:
    InternalParsingException(const std::string& msg)
      : Exception(ExceptionType::Error, "internal parsing error: " + msg)
    {}

    InternalParsingException(const std::string& msg, Position pos)
      : Exception(ExceptionType::Error,
                  "internal parsing error at " + pos.to_string() + ": " + msg)
    {}
};

class BadSyntax final : public Exception
{
  public:
    BadSyntax(const std::string& form_name)
      : Exception(ExceptionType::Error, "bad syntax: " + form_name)
    {}
    BadSyntax(const std::string& form_name, const std::string& msg)
      : Exception(ExceptionType::Error, "bad syntax: " + form_name + ": " + msg)
    {}
};

class MacroExpandingFailed final : public Exception
{
  public:
    MacroExpandingFailed()
      : Exception(ExceptionType::Error, "macro expanding failed")
    {}
    MacroExpandingFailed(const std::string& msg)
      : Exception(ExceptionType::Error, "macro expanding failed: " + msg)
    {}
};

class DefinitionException final : public Exception
{
  public:
    DefinitionException()
      : Exception(ExceptionType::Error,
                  "definition may not be used as expression")
    {}
};

class NotProcedure final : public Exception
{
  public:
    NotProcedure()
      : Exception(ExceptionType::Error, "not a procedure")
    {}
};

class UnexceptNumberOfArgument final : public Exception
{
  public:
    UnexceptNumberOfArgument(int except, int given)
      : Exception(ExceptionType::Error, format_message(except, given))
    {}

    UnexceptNumberOfArgument(int at_least, int given, bool)
      : Exception(ExceptionType::Error,
                  format_message_at_least(at_least, given))
    {}

  private:
    static inline std::string format_message(int except, int given)
    {
        std::ostringstream oss;
        oss << "unexcepted number of argument be given, ";
        oss << "excepted: " << except << " ";
        oss << "given: " << given;
        return oss.str();
    }

    static inline std::string format_message_at_least(int at_least, int given)
    {
        std::ostringstream oss;
        oss << "unexcepted number of argument be given, ";
        oss << "at least: " << at_least << " ";
        oss << "given: " << given;
        return oss.str();
    }
};

class UnquoteInvalid final : public Exception
{
  public:
    UnquoteInvalid(const std::string& msg)
      : Exception(ExceptionType::Error, "unquote invalid: " + msg)
    {}
};

class UnquoteSplicingInvalid final : public Exception
{
  public:
    UnquoteSplicingInvalid(const std::string& msg)
      : Exception(ExceptionType::Error, "unquote-splicing invalid: " + msg)
    {}
};

class UnexceptType final : public Exception
{
  public:
    UnexceptType(const std::string& except, const std::string& given)
      : Exception(ExceptionType::Error,
                  "unexcepted type be given, excepted: " + except +
                    " given: " + given)
    {}
};

} // namespace libdocscript

#endif