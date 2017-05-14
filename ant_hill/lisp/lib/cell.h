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
public:
    constexpr Nil() noexcept = default;
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

template<typename T>
std::string toString(const T& v) {
    return TypeInfo<T>::toString(v);
}

template<>
class TypeInfo<Nil>
{
public:
    static constexpr const char* name() {
        return "nil";
    }
    static std::string toString(Nil = Nil{}) {
        return name();
    }
};

template<>
class TypeInfo<Integer>
{
public:
    static constexpr const char* name() {
        return "integer";
    }
    static std::string toString(Integer v) {
        return std::to_string(v);
    }
};

template<>
class TypeInfo<Float>
{
public:
    static constexpr const char* name() {
        return "float";
    }
    static std::string toString(Float v) {
        return std::to_string(v);
    }
};

template<>
class TypeInfo<Symbol>
{
public:
    static constexpr const char* name() {
        return "standard-char";
    }
    static std::string toString(Symbol ch) {
        return std::string("#\\") + ch;
    }
};

template<>
class TypeInfo<String>
{
public:
    static constexpr const char* name() {
        return "simple-base-string";
    }
    static std::string toString(const String& str) {
        return '"' + str + '"';
    }
};

template<>
class TypeInfo<FunctionPtr>
{
public:
    static constexpr const char* name() {
        return "function";
    }
    static std::string toString(FunctionPtr ptr) {
        auto out = std::ostringstream();
        out << name() << ": " << ptr.get();
        return out.str();
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

    class BadGetError
        : public Exception
    {
    };

    template<typename T>
    const T& get() const {
        try {
            return boost::get<T>(value);
        } catch (const boost::bad_get& err) {
            throw BadGetError()
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
            throw BadGetError()
                << "Wrong type. "
                << this->toString()
                << " is not a " << TypeInfo<T>::name() << '.'
            ;
        }
    }

    template<typename T>
    using Visitor = boost::static_visitor<T>;

    template<
        typename TVisitor
        , typename TResult = typename TVisitor::result_type
    >
    TResult visit(TVisitor visitor) {
        return boost::apply_visitor(visitor, value);
    }

    template<
        typename TVisitor
        , typename TResult = typename TVisitor::result_type
    >
    TResult visit(const TVisitor& visitor) const {
        return boost::apply_visitor(visitor, value);
    }

    // template<typename TVisitor>
    // void visit(TVisitor& visitor) {
    //     boost::apply_visitor(visitor, value);
    // }

    std::string toString() const;

public:
    Value value;
};

template<>
class TypeInfo<Cons>
{
public:
    static constexpr const char* name() {
        return "cons";
    }
    static std::string toString(const Cons& cons) {
        auto out = std::ostringstream();
        out << '('
            << (cons.car ? TypeInfo<Nil>::toString() : cons.car->toString())
            << " . "
            << (cons.cdr ? TypeInfo<Nil>::toString() : cons.cdr->toString())
            << ')';
        return out.str();
    }
};

template<>
class TypeInfo<Table>
{
public:
    static constexpr const char* name() {
        return "table";
    }
    static std::string toString(const Table& table) {
        auto out = std::ostringstream();
        for (const auto& it : table) {
            out << ":" << it.first << " " << it.second->toString() << " ";
        }
        return out.str();
    }
};


}  // namespace Lisp
