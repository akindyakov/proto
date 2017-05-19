#include "basic_functions.h"


namespace Lisp {
namespace Func {

Cell If::call(Function::Args args) const {
    if (args.size() < 2) {
        throw Error() << "given too few arguments: " << args.size();
    }
    if (args.size() > 3) {
        throw Error() << "given too many arguments: " << args.size();
    }
    if (args.size() == 2) {
        args.push_back(Cell{});
    }
    return args[0].is<Nil>() ? args[2] : args[1];
}

Cell Less::call(Function::Args args) const {
    if (args.empty()) {
        throw Error() << "given too few arguments: " << args.size();
    }
    auto it = args.cbegin();
    auto prev = it;
    ++it;
    while (it != args.cend()) {
        if (!(prev->asRealNumber() < it->asRealNumber())) {
            return Cell{};
        }
        ++it;
        ++prev;
    }
    return True;
}

}  // namespace Func
}  // namespace Lisp
