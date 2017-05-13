#pragma once

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

class Cons {
public:
    CellPtr car;
    CellPtr cdr;
};

class Function
{
public:
    using Args = std::vector<CellPtr>;
    virtual CellPtr call(Args args) const = 0;
};

using FunctionPtr = std::shared_ptr<Function>;

using Integer = long;
using Float = double;
using String = std::string;
using Symbol = String::value_type;
using Table = std::unordered_map<String, CellPtr>;


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

    template<typename T>
    const T& get() const {
        return boost::get<T>(value);
    }

    template<typename T>
    T& get() {
        return boost::get<T>(value);
    }

public:
    Value value;
};

}  // namespace Lisp
