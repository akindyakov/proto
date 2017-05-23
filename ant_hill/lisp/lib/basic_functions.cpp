#include "basic_functions.h"
#include "context.h"
#include "parser.h"

#include <tools/tests/ut.h>


namespace Lisp {
namespace Func {

namespace {

class RuntimeDefinedFunction
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
    std::string body;
    Context& cnt;

public:
    explicit RuntimeDefinedFunction(
        ArgNames argNames_
        , std::string body_
        , Context& cnt_
    )
        : argNames(argNames_)
        , body(body_)
        , cnt(cnt_)
    {
    }

    Cell call(Function::Args args) const override {
        ValidateEqual(args.size(), argNames.size());
        auto local = cnt.isolateContext();
        auto nameIt = argNames.cbegin();
        for (auto& arg : args) {
            local.ns.add(*nameIt, arg.get());
            ++nameIt;
        }
        return local.eval(body);
    }

    static ArgNames readArgumentNames(const std::string& alist) {
        auto in = std::istringstream(alist);
        auto argNames = ArgNames{};

        ExprParser::readBegin(in);
        skipSpaces(in);
        auto ch = in.peek();
        while (in.good() && ch != ExprParser::CHAR_CLOSE) {
            argNames.push_back(NameParser::read(in));
            skipSpaces(in);
            ch = in.peek();
        }
        ExprParser::readEnd(in);
        return argNames;
    }
};

}

Cell Define::call(Function::Args args) const {
    /*
        (define name value) <- define var
        (define name (arg1 arg2) <body>) <- define function
        (define name (arg1 arg2) "<docstring>" <body>) <- define function
         ^       ^    ^            ^            ^
         |       |    |            |            |
         DEFINE  0    1            2            3
    */
    if (args.size() < 2) {
        throw Error() << "given too few arguments: " << args.size();
    }
    if (args.size() == 2) {
        // define local variable (global?)
        constexpr auto varNameInd  = size_t{0};
        constexpr auto varValueInd = size_t{1};
        return cnt_.ns.addGlobal(
            args[varNameInd].expr(),
            args[varValueInd].get()
        );
    }
    // define function
    constexpr auto funNameInd = size_t{0};
    constexpr auto argListInd = size_t{1};
    const auto& fname = args[funNameInd].expr();
    if (
        !ExprParser::checkPrefix(
            args[argListInd].expr()[0]
        )
    ) {  // argumets list starts with '('
        throw Error() << "argumets list starts with '('";
    }
    auto argNames = RuntimeDefinedFunction::readArgumentNames(
        args[argListInd].expr()
    );

    auto bodyFirstSegmentInd = size_t{2};
    // is here any docstring ?
    if (StringValueParser::checkPrefix(args[bodyFirstSegmentInd].expr()[0])) {
        auto docStrInd = bodyFirstSegmentInd;
        ++bodyFirstSegmentInd;
        auto in = std::istringstream(args[docStrInd].expr());
        StringValueParser::read(in);
    }
    auto body = std::string{};
    for (
        auto it = args.begin() + bodyFirstSegmentInd;
        it != args.end(); ++it
    ) {
        body += it->expr();
    }
    return cnt_.ns.addFunction(
        fname,
        std::make_unique<RuntimeDefinedFunction>(argNames, body, cnt_)
    );
}

Cell If::call(Function::Args args) const {
    if (args.size() < 2) {
        throw Error() << "given too few arguments: " << args.size();
    }
    if (args.size() > 3) {
        throw Error() << "given too many arguments: " << args.size();
    }
    if (args.size() == 2) {
        args.push_back(Cell{});
    }
    return args[0].get().is<Nil>() ? args[2].get() : args[1].get();
}

Cell Less::call(Function::Args args) const {
    if (args.empty()) {
        throw Error() << "given too few arguments: " << args.size();
    }
    auto it = args.begin();
    auto prev = it;
    ++it;
    while (it != args.end()) {
        if (!(prev->get().asRealNumber() < it->get().asRealNumber())) {
            return Cell{};
        }
        ++it;
        ++prev;
    }
    return True;
}

}  // namespace Func
}  // namespace Lisp
