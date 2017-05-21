#include "context.h"
#include "parser.h"

#include <tools/tests/ut.h>

#include <cctype>


namespace Lisp {

Cell Context::eval(std::istream& in) {
    ExprParser::readBegin(in);
    skipSpaces(in);
    auto fname = NameParser::read(in);

    auto ans = Cell{};
    // TODO: create map here
    if (fname == "defun") {
        ans = this->defun(in);
    } else {
        std::cerr << "funcal: " << fname << '\n';
        auto&& fun = globalEnv.findFunction(fname);
        auto args = this->readFunctionArguments(in);
        ans = fun->call(std::move(args));
    }
    ExprParser::readEnd(in);
    return ans;
}

Function::Args Context::readFunctionArguments(std::istream& in) {
    auto args = Function::Args{};
    skipSpaces(in);
    auto ch = in.peek();
    while (in.good() && ch != PARENT_CLOSE) {
        if (ch == PARENT_OPEN) {
            auto expr = ExprParser::read(in);
            args.push_back(
                Feature(expr, this);
            );

        } else if (RealNumberParser::checkPrefix(ch)) {
            args.push_back(RealNumberParser::read(in));

        } else if (StringValueParser::checkPrefix(ch)) {
            args.push_back(StringValueParser::read(in));

        } else if (SimpleCharacterParser::checkPrefix(ch)) {
            args.push_back(SimpleCharacterParser::read(in));

        } else {
            auto varName = NameParser::read(in);
            args.push_back(
                Feature(varName, this);
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
        last = this->eval_one(in);
        skipSpaces(in);
    }
    return last;
}

Cell Context::eval(const std::string& expr) {
    auto in = std::istringstream(expr);
    return this->eval(in);
}

Cell Context::setq(std::istream& in) {
    return Cell{};
}

Cell Context::let(std::istream& in) {
    return Cell{};
}

Cell Context::defun(std::istream& in) {
    skipSpaces(in);
    auto fname = NameParser::read(in);
    // read argument list declaration
    auto argNames = EvalInterpret::ArgNames{};
    ExprParser::readBegin(in);
    skipSpaces(in);
    while (in.good() && in.peek() != PARENT_CLOSE) {
        auto argName = NameParser::read(in);
        argNames.push_back(argName);
        skipSpaces(in);
    }
    ExprParser::readEnd(in);

    skipSpaces(in);
    if (in.good() && StringValueParser::checkPrefix(in.peek())) {
        // read doc string
        auto doc = StringValueParser::read(in);
    }
    skipSpaces(in);
    auto fbody = ExprParser::read(in);

    return globalEnv.addFunction(
        fname,
        std::make_unique<EvalInterpret>(*this, argNames, fbody)
    );
}

void Context::pushStackFrame(LocalEnv lEnv) {
    localEnv.push_back(std::move(lEnv));
}

void Context::popStackFrame() {
    localEnv.pop_back();
}

Cell Context::findName(const std::string& name) const {
    auto dst = Cell{};
    if (this->mn.findName(name, dst)) {
        return dst;
    }
    return this->externalContext->findName(name);
}

Cell Context::addName(const std::string& name, Cell value) {
    mn.addName(name, std::move(dst));
}

std::shared_ptr<Context> Context::newContext() const {
    return std::make_shared<Context>();
}

std::shared_ptr<Context> Context::nextContext() const {
    auto next = newContext();
    next.externalContext = this->shared_from_this();
    return next;
}

}  // namespace Lisp
