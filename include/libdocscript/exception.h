#ifndef LIBDOCSCRIPT_EXCEPTION_H
#define LIBDOCSCRIPT_EXCEPTION_H
#include "libdocscript/misc.h"
#include <exception>
#include <sstream>
#include <string>

namespace docscript::libdocscript {
class UnimplementException : public std::exception {};

class BasicException : public std::exception {
  public:
    enum ExceptionObject { TokenStream, Parser };

    BasicException(ExceptionObject obj, const std::string &name,
                   const std::string &msg, Position pos)
        : object(obj), name(name), message(msg), position(pos) {}
    ~BasicException() override = default;

    inline const char *what() const override {
        return msg().c_str();
    }

    inline std::string msg() const {
        std::ostringstream oss;
        oss << (object == ExceptionObject::TokenStream ? "[TokenStream]"
                                                       : "[Parser]");
        oss << " " << name << ":" << message << ", at "
            << static_cast<std::string>(position);
        return oss.str();
    }

    const std::string name;
    const std::string message;
    const Position position;
    const ExceptionObject object;
};

class TokenStreamInternalException : public BasicException {
  public:
    TokenStreamInternalException(const std::string &msg, Position p)
        : BasicException(BasicException::TokenStream, "InternalException", msg,
                         p) {}
    ~TokenStreamInternalException() override = default;
};

class TokenStreamUnknownTokenException : public BasicException {
  public:
    TokenStreamUnknownTokenException(const std::string &tokenstr, Position p)
        : BasicException(BasicException::TokenStream, "UnknownTokenException",
                         "Unknown token `" + tokenstr + "' detected", p) {}
    ~TokenStreamUnknownTokenException() override = default;
};

class ParserInternalException : public BasicException {
  public:
    ParserInternalException(const std::string &msg, Position p)
        : BasicException(BasicException::Parser, "InternalException", msg, p) {}
    ~ParserInternalException() override = default;
};

class ParserIllegalSyntaxException : public BasicException {
  public:
    ParserIllegalSyntaxException(const std::string &msg, Position p)
        : BasicException(BasicException::Parser, "IllegalSyntaxException", msg,
                         p) {}
    ~ParserIllegalSyntaxException() override = default;
};

class ParserIncompleteException : public BasicException {
  public:
    ParserIncompleteException(Position p)
        : BasicException(BasicException::Parser, "IncompleteException",
                         "Incomplete syntax", p) {}
    ~ParserIncompleteException() override = default;
};

class ParserUndefinedSyntaxException : public BasicException {
  public:
    ParserUndefinedSyntaxException(const std::string &msg, Position p)
        : BasicException(BasicException::Parser, "UndefinedSyntaxException",
                         msg, p) {}
    ~ParserUndefinedSyntaxException() override = default;
};

class ParserEOF : std::exception {};

} // namespace libdocscript

#endif