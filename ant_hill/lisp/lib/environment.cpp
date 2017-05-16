#include "environment.h"
#include "math_functions.h"

#include <cctype>


namespace Lisp {

Env::Env()
    : global{
        {"+", std::make_shared<Func::Sum>()},
        {"*",  std::make_shared<Func::Product>()},
        {"-", std::make_shared<Func::Subtract>()},
        {"/", std::make_shared<Func::Division>()},
        {"rem", std::make_shared<Func::Remainder>()},
        {"abs", std::make_shared<Func::Abs>()},
        {"max", std::make_shared<Func::Max>()},
        {"min", std::make_shared<Func::Min>()},
    }
{
}

const Function& Env::findFunction(const std::string& name) const {
    auto it = global.find(name);
    if (it == global.end()) {
        throw Error() << "There is no such a name";
    }
    return *it->second.get();
}

}  // namespace Lisp
