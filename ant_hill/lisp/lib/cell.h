#pragma once

#include <tools/exception.h>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <string>
#include <unordered_map>

namespace Lisp {

class Context;

class Cell;

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

std::ostream& operator<<(std::ostream& os, const Nil& v);

class Cons;
using ConsPtr = std::shared_ptr<Cons>;

using Integer = long;
using Float = double;
using String = std::string;
using Symbol = String::value_type;
// using Table = std::unordered_map<String, Cell>;

class Function;
using FunctionPtr = const Function*;

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
        out << name() << ": " << ptr;
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
        , ConsPtr
//        , Table
        , FunctionPtr
    >;

    enum class Tag {
        Nil,
        Integer,
        Float,
        Symbol,
        String,
        Cons,
//        Table,
        Function,
    };

    ~Cell() = default;

    class Exception
        : public ::Exception
    {
    };

    Cell()
        : value(Nil{})
    {
    }
    explicit Cell(Nil v)
        : value(v)
    {
    }
    explicit Cell(Integer v)
        : value(v)
    {
    }
    explicit Cell(Float v)
        : value(v)
    {
    }
    explicit Cell(Symbol v)
        : value(v)
    {
    }
    explicit Cell(const String& v)
        : value(v)
    {
    }
    explicit Cell(const ConsPtr& v)
        : value(v)
    {
    }
    explicit Cell(ConsPtr&& v)
        : value(std::move(v))
    {
    }
 //   explicit Cell(const Table& v)
 //       : value(v)
 //   {
 //   }
 //   explicit Cell(Table&& v)
 //       : value(std::move(v))
 //   {
 //   }
    explicit Cell(FunctionPtr ptr)
        : value(ptr)
    {
    }

    Cell(Cell&&) = default;
    Cell(const Cell&) = default;

    Cell& operator=(Cell&&) = default;
    Cell& operator=(const Cell&) = default;

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

    template<typename T>
    bool is() const {
        return this->tag() == tagOf<T>();
    }

    class BadGetError
        : public Exception
    {
    };

    Float asRealNumber() const {
        auto realV = Float{};
        if (this->is<Float>()) {
            realV = this->get<Float>();
        } else if (this->is<Integer>()) {
            realV = static_cast<Float>(
                this->get<Integer>()
            );
        } else {
            throw BadGetError() << this->toString() << "is not a real number";
        }
        return realV;
    }

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

bool operator == (
    const Cell& first
    , const Cell& second
);

bool operator != (
    const Cell& first
    , const Cell& second
);

class Cons {
public:
    Cell car;
    Cell cdr;
};

template<>
class TypeInfo<ConsPtr>
{
public:
    static constexpr const char* name() {
        return "cons";
    }
    static std::string toString(const ConsPtr& cons) {
        auto out = std::ostringstream();
        out << '('
            << (cons ? cons->car.toString() : "?")
            << " . "
            << (cons ? cons->cdr.toString() : "?")
            << ')';
        return out.str();
    }
};

const auto True = Cell{Integer{1}};

// template<>
// class TypeInfo<Table>
// {
// public:
//     static constexpr const char* name() {
//         return "table";
//     }
//     static std::string toString(const Table& table) {
//         auto out = std::ostringstream();
//         for (const auto& it : table) {
//             out << ":" << it.first << " " << it.second->toString() << " ";
//         }
//         return out.str();
//     }
// };

class ArgFuture
{
public:
    class Error
        : public ::Exception
    {
        explicit Error()
            : Exception("Future error.")
        {
        }
    };

public:
    explicit ArgFuture(std::string expr, Context* cnt);
    /*explicit*/ ArgFuture(Cell value);

    template<typename T = Cell>
    const T& get() {
        this->compute();
        return this->value_.get<T>();
    }

    const std::string& expr() const;

private:
    void compute();

private:
    Context* cnt_;
    const std::string expr_;
    Cell value_;
};

template<>
const Cell& ArgFuture::get<Cell>() {
    this->compute();
    return this->value_;
}

class Function
{
public:
    using Args = std::vector<ArgFuture>;
    virtual Cell call(Args args) const = 0;
};

}  // namespace Lisp
