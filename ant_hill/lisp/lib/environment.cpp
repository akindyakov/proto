#include "environment.h"
#include "math_functions.h"

#include <cctype>


namespace Lisp {

Env::Env()
{
    addFunction("+", std::make_unique<Func::Sum>());
    addFunction("*",  std::make_unique<Func::Product>());
    addFunction("-", std::make_unique<Func::Subtract>());
    addFunction("/", std::make_unique<Func::Division>());
    addFunction("rem", std::make_unique<Func::Remainder>());
    addFunction("abs", std::make_unique<Func::Abs>());
    addFunction("max", std::make_unique<Func::Max>());
    addFunction("min", std::make_unique<Func::Min>());
}

FunctionPtr Env::findFunction(const std::string& name) const {
    auto it = funcs.find(name);
    if (it == funcs.end()) {
        throw Error() << "There is no function with name '" << name << "'";
    }
    return it->second.get();
}

Cell Env::addFunction(
    const std::string& name
    , std::unique_ptr<Function> fun
) {
    if (funcs.count(name)) {
        funcs.erase(name);
        names.erase(name);
    }
    funcs.emplace(name, std::move(fun));
    names.emplace(name, Cell(funcs[name].get()));
    return findName(name);
}

const Cell& Env::findName(const std::string& name) const {
    auto it = names.find(name);
    if (it == names.end()) {
        throw Error() << "There is no such a name '" << name << "'";
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
