#include "environment.h"
#include "math_functions.h"

#include <cctype>


namespace Lisp {

Env::Env()
{
    funcs.emplace("+", std::make_unique<Func::Sum>());
    funcs.emplace("*",  std::make_unique<Func::Product>());
    funcs.emplace("-", std::make_unique<Func::Subtract>());
    funcs.emplace("/", std::make_unique<Func::Division>());
    funcs.emplace("rem", std::make_unique<Func::Remainder>());
    funcs.emplace("abs", std::make_unique<Func::Abs>());
    funcs.emplace("max", std::make_unique<Func::Max>());
    funcs.emplace("min", std::make_unique<Func::Min>());
}

FunctionPtr Env::findFunction(const std::string& name) const {
    auto it = funcs.find(name);
    if (it == funcs.end()) {
        throw Error() << "There is no such a function name";
    }
    return it->second.get();
}

const Cell& Env::findName(const std::string& name) const {
    auto it = names.find(name);
    if (it == names.end()) {
        throw Error() << "There is no such a name";
    }
    return it->second;
}

Cell& Env::findName(const std::string& name) {
    auto it = names.find(name);
    if (it == names.end()) {
        throw Error() << "There is no such a name";
    }
    return it->second;
}

Cell& Env::addName(const std::string& name, Cell value) {
    return names[name] = std::move(value);
}

Cell Env::popName(const std::string& name) {
    auto it = names.find(name);
    if (it == names.end()) {
        throw Error() << "There is no such a name";
    }
    auto v = std::move(it->second);
    names.erase(name);
    return v;
}

}  // namespace Lisp
