#include "evaluate.h"
#include "parser.h"

#include <tools/tests/ut.h>

#include <cctype>


namespace Lisp {

namespace {

static inline void skipSpaces(std::istream& in) {
    while (in && std::isspace(in.peek())) {
        in.ignore();
    }
}

class EvalInterpret
    : public Function
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("Eval: ")
        {
        }
    };


private:
    using ArgNames = std::vector<std::string>;
    Main& glob;
    ArgNames argNames;
    std::string body;

private:
    explicit EvalInterpret(
        Main& glob_
        , ArgNames argNames_
        , std::string body_
    )
        : glob(glob_)
        , argNames(argNames_)
        , body(body_)
    {
    }

public:

    Cell call(Function::Args args) const override {
        auto localEnv = LocalEnv{};
        ValidateEqual(args.size(), argNames.size());
        auto nameIt = argNames.cbegin();
        for (const auto& val : args) {
            localEnv.emplace(*nameIt, std::move(val));
        }
        glob.pushStackFrame(std::move(localEnv));
        auto ret = glob.eval(body);
        glob.popStackFrame();
        return ret;
    }

};

}


Cell Main::eval(std::istream& in) {
    skipSpaces(in);
    auto ch = char{};
    in.get(ch);
    if (ch != '(') {
        throw Error();
    }
    skipSpaces(in);
    auto fname = NameParser::read(in);
    //*dbg*/ std::cerr << "fun: " << fname << '\n';

    // TODO: create map here
    if (fname == "defun") {
        return this->defun(in);
    }
    // else {
    auto&& fun = globalEnv.findFunction(fname);
    auto args = this->readFunctionArguments(in);
    //*dbg*/ for (const auto& arg : args) {
    //*dbg*/     std::cerr << arg.toString() << '\n';
    //*dbg*/ }
    return fun->call(args);
}

Function::Args Main::readFunctionArguments(std::istream& in) {
    auto args = Function::Args{};
    skipSpaces(in);
    auto ch = in.peek();
    while (in && ch != ')') {
        if (ch == '(') {
            args.push_back(this->eval(in));

        } else if (RealNumberParser::checkPrefix(ch)) {
            args.push_back(RealNumberParser::read(in));

        } else if (StringValueParser::checkPrefix(ch)) {
            args.push_back(StringValueParser::read(in));

        } else if (SimpleCharacterParser::checkPrefix(ch)) {
            args.push_back(SimpleCharacterParser::read(in));

        } else {
            auto varName = NameParser::read(in);
            args.push_back(
                this->findName(varName)
            );

        }
        skipSpaces(in);
        ch = in.peek();
    }
    // drop last brace
    in.ignore();
    return args;
}

Cell Main::eval(const std::string& expr) {
    auto in = std::istringstream(expr);
    return this->eval(in);
}

Cell Main::setq(std::istream& in) {
    return Cell{};
}

Cell Main::let(std::istream& in) {
    return Cell{};
}

Cell Main::defun(std::istream& in) {
    skipSpaces(in);
    auto fname = NameParser::read(in);
    skipSpaces(in);
    // read argument list
    auto argNames = EvalInterpret::ArgNames{};
    if (in && in.get() == '(') {
        skipSpaces(in);
        while (in && in.peek() != ')') {
            argNames.push_back(NameParser::read(in));
            skipSpaces(in);
        }
    } else {
        throw Error();
    }
    skipSpaces(in);
    if (in && StringValueParser::checkPrefix(in.peek())) {
        // read doc string
        StringValueParser::read(in);
    }
    skipSpaces(in);
    auto counter = int{0};
    auto fbody = std::string{};
    if (in && in.get() != '(') {
        throw Error();
    }
    ++counter;
    while (in && counter) {
        auto ch = char{};
        in.get(ch);
        counter += ch == '(' ? 1 : 0;
        counter -= ch == ')' ? 1 : 0;
        fbody.push_back(ch);
    }
    // drop last
    skipSpaces(in);
    if (in.get() != ')') {
        throw Error();
    }
    return globalEnv.addFunction(
        fname,
        std::make_unique<EvalInterpret>(glob, argNames, fbody)
    );
}

void Main::pushStackFrame(LocalEnv lEnv) {
    localEnv.push_back(std::move(lEnv));
}

void popStackFrame() {
    localEnv.pop_back();
}

const Cell& findName(const std::string& name) const {
    for (
        const auto& envIt = this->localEnv.rcbegin();
        envIt !+ this->localEnv.rcend(); ++envIt
    ) {
        auto cit = envIt->find(name);
        if (cit != envIt->cend()) {
            return *cit;
        }
    }
    return this->globalEnv.findName(name);
}

Cell& findName(const std::string& name) {
    for (
        auto& envIt = this->localEnv.rbegin();
        envIt !+ this->localEnv.rend(); ++envIt
    ) {
        auto cit = envIt->find(name);
        if (cit != envIt->end()) {
            return *cit;
        }
    }
    return this->globalEnv.findName(name);
}

}  // namespace Lisp
