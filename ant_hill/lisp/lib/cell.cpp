#include "cell.h"

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

}  // namespace Lisp
