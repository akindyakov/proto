#include "math_functions.h"


namespace Lisp {
namespace Func {

Cell Abs::call(Function::Args args) const {
    if (args.empty()) {
        throw AbsError() << "given too few arguments";
    }
    if (args.size() > 1) {
        throw AbsError() << "given too many arguments";
    }
    if (args.front().is<Float>()) {
        auto v = args.front().get<Float>();
        v = v < 0 ? -v : v;
        return Cell(v);
    } else if (args.front().is<Integer>()) {
        auto v = args.front().get<Integer>();
        v = v < 0 ? -v : v;
        return Cell(v);
    }
    throw AbsError() << args.front().toString() << "is not a real number";
}

namespace {

bool realNumberGreater(
    const Cell& first
    , const Cell& second
) {
    return first.asRealNumber() > second.asRealNumber();
}

}
Cell Max::call(Function::Args args) const {
    if (args.empty()) {
        throw MaxError() << "given too few arguments";
    }
    auto it = args.cbegin();
    auto maxValue = it;
    ++it;
    while (it != args.cend()) {
        if (realNumberGreater(*it, *maxValue)) {
            maxValue = it;
        }
        ++it;
    }
    return *maxValue;
}

namespace {

bool realNumberLess(
    const Cell& first
    , const Cell& second
) {
    return first.asRealNumber() < second.asRealNumber();
}

}

Cell Min::call(Function::Args args) const {
    if (args.empty()) {
        throw MinError() << "given too few arguments";
    }
    auto it = args.cbegin();
    auto minValue = it;
    ++it;
    while (it != args.cend()) {
        if (realNumberLess(*it, *minValue)) {
            minValue = it;
        }
        ++it;
    }
    return *minValue;
}

}  // namespace Func
}  // namespace Lisp
