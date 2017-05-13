#include "cell.h"

namespace Lisp {

namespace {

class ToStringVisitor
    : public Cell::Visitor<std::string>
{
    std::string operator(Nil) const {
        return TypeInfo<Nil>::name();
    }
    std::string operator(Integer v) const {
        return std::to_string(v);
    }
    std::string operator(Float v) const {
        return std::to_string(v);
    }
    std::string operator(Symbol v) const {
        return std::string(v);
    }
    String,
    Cons,
    Table,
    Function,
};

}

std::string Cell::toString() const {
    // FIXME
    return std::to_string(
        static_cast<std::underlying_type<Tag>::type>(
            this->tag()
        )
    );
}

}  // namespace Lisp
