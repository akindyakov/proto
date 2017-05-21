#include "cell.h"
#include "context.h"

namespace Lisp {

namespace {

class ToStringVisitor
{
public:
    using result_type = std::string;

public:
    template<typename T>
    std::string operator()(const T& v) const {
        return toString(v);
    }
};

}

std::string Cell::toString() const {
    return this->visit(
        ToStringVisitor()
    );
}

bool operator == (
    const Cell& first
    , const Cell& second
) {
    return first.value == second.value;
}

std::ostream& operator<<(std::ostream& os, const Nil& v) {
    os << toString(v);
    return os;
}

bool operator != (
    const Cell& first
    , const Cell& second
) {
    return !(first == second);
}

Future::Future(std::string expr, Context* cnt)
    : cnt_(cnt)
    , expr_(expr)
    , value_(Cell{})
{
}

Future::Future(Cell value)
    : cnt_(nullptr)
    , value_(std::move(value))
{
}

const std::string& Future::expr() const {
    return this->expr_;
}

void Future::compute() {
    if (cnt_ == nullptr) {
        this->value_ = this->cnt_->eval(
            this->expr_
        );
    }
    this->cnt_ = nullptr;
}


}  // namespace Lisp
