#include "environment.h"
#include "math_functions.h"

#include <cctype>


namespace Lisp {

Env::Env()
{
    global.emplace("+", std::make_unique<Func::Sum>());
    global.emplace("*",  std::make_unique<Func::Product>());
    global.emplace("-", std::make_unique<Func::Subtract>());
    global.emplace("/", std::make_unique<Func::Division>());
    global.emplace("rem", std::make_unique<Func::Remainder>());
    global.emplace("abs", std::make_unique<Func::Abs>());
    global.emplace("max", std::make_unique<Func::Max>());
    global.emplace("min", std::make_unique<Func::Min>());
}

FunctionPtr Env::findFunction(const std::string& name) const {
    auto it = global.find(name);
    if (it == global.end()) {
        throw Error() << "There is no such a name";
    }
    return it->second.get();
}

}  // namespace Lisp
