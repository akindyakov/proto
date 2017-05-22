#include "context.h"
#include "namespace.h"
#include "parser.h"
#include "basic_functions.h"

#include <tools/tests/ut.h>

#include <cctype>


namespace Lisp {

Context::Context()
    : ns(Namespace::createGlobal())
{
    ns.addFunction("define", std::make_unique<Func::Define>(*this));
}

Cell Context::eval(std::istream& in) {
    auto ans = Cell{};
    skipSpaces(in);
    if (in.good() && ExprParser::checkPrefix(in.peek())) {
        ExprParser::readBegin(in);

        skipSpaces(in);
        auto fname = NameParser::read(in);
        std::cerr << "funcal: " << fname << '\n';
        auto funcPtr = ns.findFunction(fname);
        if (funcPtr == nullptr) {
            throw Error() << "Function not found";
        }
        auto args = this->readFunctionArguments(in);
        ans = funcPtr->call(std::move(args));

        ExprParser::readEnd(in);
    } else {
        // just name
        std::cerr << "just find the name\n";
        ans = ns.find(NameParser::read(in));
    }
    return ans;
}

Function::Args Context::readFunctionArguments(std::istream& in) {
    auto args = Function::Args{};
    skipSpaces(in);
    auto ch = in.peek();
    while (in.good() && ch != PARENT_CLOSE) {
        std::cerr << "read arg: '" << char(ch) << "'\n";
        if (ch == PARENT_OPEN) {
            std::cerr << "expr\n";
            args.push_back(
                ArgFuture(ExprParser::read(in), this)
            );

        } else if (RealNumberParser::checkPrefix(ch)) {
            std::cerr << "number\n";
            args.push_back(RealNumberParser::read(in));

        } else if (StringValueParser::checkPrefix(ch)) {
            std::cerr << "string\n";
            args.push_back(StringValueParser::read(in));

        } else if (SimpleCharacterParser::checkPrefix(ch)) {
            std::cerr << "char\n";
            args.push_back(SimpleCharacterParser::read(in));

        } else {
            std::cerr << "name\n";
            args.push_back(
                ArgFuture(NameParser::read(in), this)
            );

        }
        skipSpaces(in);
        ch = in.peek();
    }
    return args;
}

Cell Context::eval_all(const std::string& expr) {
    auto in = std::istringstream(expr);
    return this->eval(in);
}

Cell Context::eval_all(std::istream& in) {
    auto last = Cell{};
    skipSpaces(in);
    while (in.good()) {
        last = this->eval(in);
        skipSpaces(in);
    }
    return last;
}

Cell Context::eval(const std::string& expr) {
    auto in = std::istringstream(expr);
    return this->eval(in);
}

// Cell Context::setq(std::istream& in) {
//     return Cell{};
// }
//
// Cell Context::let(std::istream& in) {
//     return Cell{};
// }
//
// Cell Context::defun(std::istream& in) {
//     skipSpaces(in);
//     auto fname = NameParser::read(in);
//     // read argument list declaration
//     auto argNames = EvalInterpret::ArgNames{};
//     ExprParser::readBegin(in);
//     skipSpaces(in);
//     while (in.good() && in.peek() != PARENT_CLOSE) {
//         auto argName = NameParser::read(in);
//         argNames.push_back(argName);
//         skipSpaces(in);
//     }
//     ExprParser::readEnd(in);
//
//     skipSpaces(in);
//     if (in.good() && StringValueParser::checkPrefix(in.peek())) {
//         // read doc string
//         auto doc = StringValueParser::read(in);
//     }
//     skipSpaces(in);
//     auto fbody = ExprParser::read(in);
//
//     return ns.addFunction(
//         fname,
//         std::make_unique<EvalInterpret>(*this, argNames, fbody)
//     );
// }
//
// void Context::pushStackFrame(LocalEnv lEnv) {
//     localEnv.push_back(std::move(lEnv));
// }
//
// void Context::popStackFrame() {
//     localEnv.pop_back();
// }

// Cell Context::findName(const std::string& name) const {
//     auto dst = Cell{};
//     if (this->ns.find(name, dst)) {
//         return dst;
//     }
//     if (this->externalContext == nullptr) {
//         throw Error() << "There is no such a name in context '" << name << "'";
//     }
//     return this->externalContext->findName(name);
// }

// Cell Context::addName(const std::string& name, Cell value) {
//     return nm.add(name, std::move(value));
// }

}  // namespace Lisp
