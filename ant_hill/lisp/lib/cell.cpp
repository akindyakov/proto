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

std::string Cell::_toString() const {
    return this->visit(
        ToStringVisitor()
    );
}

}  // namespace Lisp
