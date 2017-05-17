#include "evaluate.h"
#include "parser.h"

#include <cctype>


namespace Lisp {

namespace {

static inline void skipSpaces(std::istream& in) {
    while (in && std::isspace(in.peek())) {
        in.ignore();
    }
}

class EvalCode
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
    ArgNames argNames;

public:
    explicit EvalCode(
        ArgNames names
        , Main& 
    )
        : argNames(std::move(names))
    {
    }

    Cell call(const Function::Args& args) const override {
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
    auto&& fun = env.findFunction(fname);
    auto args = Function::Args{};

    skipSpaces(in);
    ch = in.peek();
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
                env.findName(varName)
            );

        }
        skipSpaces(in);
        ch = in.peek();
    }
    // drop last brace
    in.ignore();

    //*dbg*/ for (const auto& arg : args) {
    //*dbg*/     std::cerr << arg.toString() << '\n';
    //*dbg*/ }
    return fun->call(args);
}

Cell Main::eval(const std::string& expr) {
    auto in = std::istringstream(expr);
    return this->eval(in);
}

void Main::defun(std::istream& in) {
    skipSpaces(in);
    auto fname = NameParser::read(in);
    skipSpaces(in);
    // read argument list
    auto argNames = std::vector<std::string>{};
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
}
void Main::defvar(std::istream& in) {
}
void Main::setq(std::istream& in) {
}
void Main::let(std::istream& in) {
}
void Main::flet(std::istream& in) {
}

}  // namespace Lisp
