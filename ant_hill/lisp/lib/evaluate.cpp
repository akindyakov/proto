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
        std::cerr << args.size() << ") " << args.begin()->toString() << '\n';
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
        std::cerr << "ret: " << ret.toString() << '\n';
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
    readParenthesesBegin(in);
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
    readParenthesesEnd(in);
    return ans;
}

void Main::readParenthesesBegin(std::istream& in) {
    skipSpaces(in);
    if (!in.good()) {
        throw Error() << "Unexpected end of file at the begining of parentheses group";
    }
    char ch = in.get();
    if (ch != PARENT_OPEN) {
        throw Error() << "Wrong parentheses group first character: '" << ch << "'\n";
    }
}

void Main::readParenthesesEnd(std::istream& in) {
    skipSpaces(in);
    if (!in.good()) {
        throw Error() << "Unexpected end of file at the end of parentheses group";
    }
    char ch = in.get();
    if (ch != PARENT_CLOSE) {
        throw Error()
            << "Wrong parentheses group last character: '" << ch << "'\n";
    }
}

Function::Args Main::readFunctionArguments(std::istream& in) {
    auto args = Function::Args{};
    skipSpaces(in);
    auto ch = in.peek();
    while (in.good() && ch != PARENT_CLOSE) {
        if (ch == PARENT_OPEN) {
            std::cerr << "eval smth inside\n";
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
    // read argument list declaration
    auto argNames = EvalInterpret::ArgNames{};
    readParenthesesBegin(in);
    skipSpaces(in);
    while (in.good() && in.peek() != PARENT_CLOSE) {
        auto argName = NameParser::read(in);
        argNames.push_back(argName);
        skipSpaces(in);
    }
    readParenthesesEnd(in);

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
