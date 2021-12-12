#ifndef LIBDOCSCRIPT_UTILITY_STRINGSTREAM_H
#define LIBDOCSCRIPT_UTILITY_STRINGSTREAM_H
#include "libdocscript/utility/position.h"
#include <stack>
#include <string>

namespace libdocscript {
class BasicStringStream
{
  public:
    BasicStringStream() = default;

    BasicStringStream(const BasicStringStream &) = delete;
    BasicStringStream(BasicStringStream &&) = delete;

    BasicStringStream &operator=(const BasicStringStream &) = delete;
    BasicStringStream &operator=(BasicStringStream &&) = delete;

    virtual char get() = 0;
    virtual char peek() const = 0;
    virtual void unget() = 0;
    virtual void ignore() = 0;
    virtual std::string getline() = 0;
    virtual void reset() = 0;
    virtual void reset(const std::string &) = 0;
    virtual bool eof() const = 0;
    virtual bool last() const = 0;
    virtual const Position& position() const = 0;

    virtual bool next() = 0;
    virtual char peek_next() const = 0;

    virtual void push_back(char) = 0;
    virtual void push_back(const std::string&) = 0;

    virtual operator bool() const = 0;
};

class StringStream final : public BasicStringStream
{
  public:
    StringStream();
    StringStream(const std::string& content);

    char get() override;
    char peek() const override;
    void unget() override;
    void ignore() override;
    std::string getline() override;
    void reset() override;
    void reset(const std::string &) override;
    bool eof() const override;
    bool last() const override;
    const Position& position() const override;

    bool next() override;
    char peek_next() const override;

    void push_back(char) override;
    void push_back(const std::string&) override;

    operator bool() const override;

  private:
    std::string _content;
    std::string::const_iterator _begin;
    std::string::const_iterator _iter;
    std::string::const_iterator _end;

    Position _pos;
    std::stack<Position::value_type> _line_col_cnt;

    void increase_position();
    void decrease_position();
};
} // namespace libdocscript

#endif