#include "namespace.h"
#include "math_functions.h"
#include "basic_functions.h"

#include <cctype>


namespace Lisp {

Namespace Namespace::createGlobal() {
    auto st = std::make_unique<FunctionStorage>();
    st->add("+", std::make_unique<Func::Sum>());
    st->add("*",  std::make_unique<Func::Product>());
    st->add("-", std::make_unique<Func::Subtract>());
    st->add("/", std::make_unique<Func::Division>());
    st->add("rem", std::make_unique<Func::Remainder>());
    st->add("abs", std::make_unique<Func::Abs>());
    st->add("max", std::make_unique<Func::Max>());
    st->add("min", std::make_unique<Func::Min>());
    st->add("if", std::make_unique<Func::If>());
    st->add("<", std::make_unique<Func::Less>());
    st->add("not", std::make_unique<Func::Not>());
    return Namespace(nullptr, std::move(st));
}

Namespace Namespace::createLocal() {
    return Namespace(this);
}

Namespace Namespace::createIsolate() {
    return Namespace(
        this->findGlobal()
    );
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
    auto fptr = FunctionPtr{nullptr};
    if (this->funcs) {
        fptr = this->funcs->get(name);
    }
    if (this->ext != nullptr) {
        fptr = this->ext->findFunction(name);
    }
    if (fptr == nullptr) {
        throw Error() << "There is no such a function with name '" << name << "'";
    }
    return fptr;
}

Cell Namespace::addFunction(
    const std::string& name
    , std::unique_ptr<Function> fun
) {
    return Cell(
        this->funcs->add(
            name,
            std::move(fun)
        )
    );
}

Cell Namespace::find(const std::string& name) const {
    auto it = names.find(name);
    if (it == names.end()) {
        auto fptr = funcs->get(name);
        if (fptr == nullptr) {
            if (this->ext == nullptr) {
                throw Error() << "There is no such a name '" << name << "'";
            } else {
                return this->ext->find(name);
            }
        }
        return Cell(fptr);
    }
    return it->second;
}

bool Namespace::find(const std::string& name, Cell& dst) const {
    auto it = names.find(name);
    if (it == names.end()) {
        auto fptr = funcs->get(name);
        if (fptr == nullptr) {
            if (this->ext == nullptr) {
                return false;
            } else {
                return this->ext->find(name, dst);
            }
        }
        dst = Cell(fptr);
    } else {
        dst = it->second;
    }
    return true;
}

const Cell& Namespace::add(const std::string& name, Cell value) {
    return names[name] = std::move(value);
}

const Cell& Namespace::addGlobal(const std::string& name, Cell value) {
    auto glob = this->findGlobal();
    return glob->add(name, std::move(value));
}

void Namespace::pop(const std::string& name) {
    names.erase(name);
}

Namespace* Namespace::findGlobal() {
    Namespace* glob = this;
    while (glob->ext != nullptr) {
        glob = this->ext;
    }
    return glob;
}

const Namespace* Namespace::findGlobal() const {
    const Namespace* glob = this;
    while (glob->ext != nullptr) {
        glob = this->ext;
    }
    return glob;
}

}  // namespace Lisp
