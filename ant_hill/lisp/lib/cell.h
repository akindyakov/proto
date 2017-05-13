#pragma once

#include <tools/exception.h>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <string>
#include <unordered_map>

namespace Lisp {

class Cell;
using CellPtr = std::shared_ptr<Cell>;

class Nil
{
};

constexpr inline bool operator == (
    Nil first
    , Nil second
) noexcept {
    return true;
}

class Cons {
public:
    CellPtr car;
    CellPtr cdr;
};

using Integer = long;
using Float = double;
using String = std::string;
using Symbol = String::value_type;
using Table = std::unordered_map<String, CellPtr>;

class Function
{
public:
    using Args = std::vector<CellPtr>;
    virtual CellPtr call(Args args) const = 0;
};

using FunctionPtr = std::shared_ptr<Function>;

template<typename T>
class TypeInfo;

template<>
class TypeInfo<Nil>
{
public:
    static constexpr const char* name() {
        return "nil";
    }
};

template<>
class TypeInfo<Integer>
{
public:
    static constexpr const char* name() {
        return "nil";
    }
};

template<>
class TypeInfo<Float>
{
public:
    static constexpr const char* name() {
        return "nil";
    }
};

template<>
class TypeInfo<Symbol>
{
public:
    static constexpr const char* name() {
        return "standard-char";
    }
};

template<>
class TypeInfo<String>
{
public:
    static constexpr const char* name() {
        return "simple-base-string";
    }
};

template<>
class TypeInfo<Cons>
{
public:
    static constexpr const char* name() {
        return "cons";
    }
};

template<>
class TypeInfo<Table>
{
public:
    static constexpr const char* name() {
        return "table";
    }
};

template<>
class TypeInfo<FunctionPtr>
{
public:
    static constexpr const char* name() {
        return "function";
    }
};

class Cell
{
public:
    using Value = boost::variant<
        Nil
        , Integer
        , Float
        , Symbol
        , String
        , Cons
        , Table
        , FunctionPtr
    >;

    enum class Tag {
        Nil,
        Integer,
        Float,
        Symbol,
        String,
        Cons,
        Table,
        Function,
    };

    class Exception
        : public ::Exception
    {
    };

    template<typename T>
    explicit Cell(const T& v)
        : value(v)
    {
    }

    Tag tag() const {
        return static_cast<Tag>(
            value.which()
        );
    }

    template<typename T>
    static Tag tagOf() {
        return static_cast<Tag>(
            Value(T{}).which()
        );
    }

    class BadGet
        : public Exception
    {
    };

    template<typename T>
    const T& get() const {
        try {
            return boost::get<T>(value);
        } catch (const boost::bad_get& err) {
            throw BadGet()
                << "Wrong type. "
                << this->toString()
                << " is not a " << TypeInfo<T>::name() << '.'
            ;
        }
    }

    template<typename T>
    T& get() {
        try {
            return boost::get<T>(value);
        } catch (const boost::bad_get& err) {
            throw BadGet()
                << "Wrong type. "
                << this->toString()
                << " is not a " << TypeInfo<T>::name() << '.'
            ;
        }
    }

    template<typename T>
    using Visitor = boost::static_visitor<T>;

    template<typename TVisitor>
    typename TVisitor::result_type visit(TVisitor& visitor) {
        return boost::apply_visitor(visitor, value);
    }

    std::string toString() const {
        // FIXME
        return std::to_string(
            static_cast<std::underlying_type<Tag>::type>(
                this->tag()
            )
        );
    }

public:
    Value value;
};

}  // namespace Lisp
