#include "namespace.h"
#include "math_functions.h"
#include "basic_functions.h"

#include <cctype>


namespace Lisp {

FunctionStorage FunctionStorage::globalStorage() {
{
    auto st = FunctionStorage{};
    st.add("+", std::make_unique<Func::Sum>());
    st.add("*",  std::make_unique<Func::Product>());
    st.add("-", std::make_unique<Func::Subtract>());
    st.add("/", std::make_unique<Func::Division>());
    st.add("rem", std::make_unique<Func::Remainder>());
    st.add("abs", std::make_unique<Func::Abs>());
    st.add("max", std::make_unique<Func::Max>());
    st.add("min", std::make_unique<Func::Min>());
    st.add("if", std::make_unique<Func::If>());
    st.add("<", std::make_unique<Func::Less>());
    return st;
}

FunctionPtr FunctionStorage::get(const std::string& name) const {
    auto it = funcs.find(name);
    if (it == funcs.end()) {
        return nullptr;
    }
    return it->second.get();
}

FunctionPtr FunctionStorage::add(
    const std::string& name
    , std::unique_ptr<Function> fun
) {
    // if (funcs.count(name)) {
    //     // throw ??
    // }
    funcs.emplace(name, std::move(fun));
    return fun.get();
}

FunctionPtr Namespace::findFunction(const std::string& name) const {
    reuturn this->funcs->get(name);
}

Cell Namespace::addFunction(
    const std::string& name
    , std::unique_ptr<Function> fun
) {
    return Cell(this->funcs->addFunction(name));
}

const Cell& Namespace::find(const std::string& name) const {
    auto it = names.find(name);
    if (it == names.end()) {
        auto fptr = funcs->find(name);
        if (fptr == nullptr) {
            throw Error() << "There is no such a name '" << name << "'";
        }
        return Cell(fptr);
    }
    return it->second;
}

bool Namespace::find(const std::string& name, Cell& dst) const {
    auto it = names.find(name);
    if (it == names.end()) {
        auto fptr = funcs->find(name);
        if (fptr == nullptr) {
            return false;
        }
        dst = Cell(fptr);
    } else {
        dst = it->second;
    }
    return true;
}

Cell& Namespace::add(const std::string& name, Cell value) {
    return names[name] = std::move(value);
}

void Namespace::pop(const std::string& name) {
    names.erase(name);
}

namespace {

static globalStorage = FunctionStorage::globalStorage();

}

Namespace::Namespace() {
    return Namespace(&globalStorage);
}

}  // namespace Lisp
