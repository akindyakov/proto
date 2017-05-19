#include "evaluate.h"
#include "parser.h"

#include <tools/tests/ut.h>

#include <cctype>


namespace Lisp {

namespace {

static inline void skipSpaces(std::istream& in) {
    while (in.good() && std::isspace(in.peek())) {
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

    using ArgNames = std::vector<std::string>;

private:
    Main& glob;
    ArgNames argNames;
    std::string body;

public:
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

    Cell call(Function::Args args) const override {
        auto localEnv = LocalEnv{};
        ValidateEqual(args.size(), argNames.size());
        auto nameIt = argNames.cbegin();
        for (const auto& val : args) {
            localEnv.emplace(*nameIt, std::move(val));
            ++nameIt;
        }
        glob.pushStackFrame(std::move(localEnv));
        auto ret = glob.eval(body);
        glob.popStackFrame();
        return ret;
    }
};

}


Cell Main::eval(std::istream& in) {
    auto last = Cell{};
    skipSpaces(in);
    while (in.good()) {
        last = this->eval_one(in);
        skipSpaces(in);
    }
    return last;
}

Cell Main::eval_one(std::istream& in) {
    skipSpaces(in);
    if (!in.good()) {
        throw Error() << "Unexpected end of file at the begining";
    }
    char ch = in.get();
    if (ch != '(') {
        throw Error() << "Wrong first character: '" << ch << "'\n";
    }

    skipSpaces(in);
    auto fname = NameParser::read(in);

    auto ans = Cell{};
    // TODO: create map here
    if (fname == "defun") {
        ans = this->defun(in);
    } else {
        auto&& fun = globalEnv.findFunction(fname);
        auto args = this->readFunctionArguments(in);
        ans = fun->call(args);
    }

    skipSpaces(in);
    // drop last brace
    if (!in.good()) {
        throw Error() << "Unexpected end of file at the end";
    }
    ch = in.get();
    if (ch != ')') {
        throw Error() << "Wrong last character: '" << ch << "'\n";
    }
    return ans;
}

Function::Args Main::readFunctionArguments(std::istream& in) {
    auto args = Function::Args{};
    skipSpaces(in);
    auto ch = in.peek();
    while (in.good() && ch != ')') {
        if (ch == '(') {
            args.push_back(this->eval_one(in));

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
    auto ch = char{};
    if (in.good() && in.get(ch) && ch == '(') {
        skipSpaces(in);
        while (in.good() && in.peek() != ')') {
            auto argName = NameParser::read(in);
            argNames.push_back(argName);
            skipSpaces(in);
        }
        in.ignore();  // skip ')'
    } else {
        throw Error() << "defun: argument list should to starts with '('";
    }
    skipSpaces(in);
    if (in.good() && StringValueParser::checkPrefix(in.peek())) {
        // read doc string
        auto doc = StringValueParser::read(in);
    }
    skipSpaces(in);
    auto counter = int{0};
    auto fbody = std::string{};
    if (in.good() && in.get(ch) && ch != '(') {
        throw Error() << "defun: function body should starts with '" << '(' << "'";
    }
    fbody.push_back(ch);
    ++counter;
    while (in.good() && counter != 0) {
        auto ch = char{};
        in.get(ch);
        counter += ch == '(' ? 1 : 0;
        counter -= ch == ')' ? 1 : 0;
        fbody.push_back(ch);
    }
    return globalEnv.addFunction(
        fname,
        std::make_unique<EvalInterpret>(*this, argNames, fbody)
    );
}

void Main::pushStackFrame(LocalEnv lEnv) {
    localEnv.push_back(std::move(lEnv));
}

void Main::popStackFrame() {
    localEnv.pop_back();
}

Cell Main::findName(const std::string& name) const {
    for (
        auto envIt = this->localEnv.crbegin();
        envIt != this->localEnv.crend(); ++envIt
    ) {
        auto cit = envIt->find(name);
        if (cit != envIt->cend()) {
            return cit->second;
        }
    }
    return this->globalEnv.findName(name);
}

}  // namespace Lisp
