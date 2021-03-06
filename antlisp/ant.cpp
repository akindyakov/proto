#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <memory>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>


class FunctionDefinition;
class FunctionCall;

class IExtFunction;

using FunctionDefinitionPtr =    std::shared_ptr<FunctionDefinition>;
using PostponedFunctionCallPtr = std::shared_ptr<FunctionCall>;
using ExtFunctionPtr =    std::shared_ptr<IExtFunction>;

struct Nil {};

using Cell = boost::variant<
    Nil
    , int
    , FunctionDefinitionPtr
    , ExtFunctionPtr
>;

bool isNil(const Cell& cell) {
    return cell.which() == 0;
}

// may we use a std::stringview here?
using TVarName = std::string;

using LocalFrame = std::vector<Cell>;
using GlobalFrame = std::unordered_map<TVarName, Cell>;

class IExtFunction {
public:
    virtual void call(
        LocalFrame& frame
    ) const = 0;
};

class ExtSum
    : public IExtFunction
{
    void call(
        LocalFrame& frame
    ) const override {
        auto sum = int{0};
        for (const auto& cell : frame) {
            sum += boost::get<int>(cell);
        }
        frame.clear();
        frame.push_back(
            Cell(sum)
        );
    }
};

class ExtPrint
    : public IExtFunction
{
    void call(
        LocalFrame& frame
    ) const override {
        for (const auto& cell : frame) {
            std::cout << boost::get<int>(cell) << '\n';
        }
        frame.clear();
    }
};

struct Environment;

struct FunctionDefinition {
    enum EOperations {
        Nope,
        GetConst,
        GetLocal,
        GetGlobal,
        SetGlobal,
        RunFunction,
        RunExternalFunction,
        StackRewind,
        SkipIfTrue,
        SkipIfFalse,
    };
    struct Step {
        explicit Step(
            EOperations op
            , std::size_t pos
        )
            : operation(op)
            , position(pos)
        {
        }
        EOperations operation;
        std::size_t position = 0;
    };

    std::vector<Step> operations;
    std::size_t argnum;
    std::vector<TVarName> names;
    LocalFrame consts;

    static bool step(Environment& env);
};

class FunctionCall {
public:
    explicit FunctionCall(
        FunctionDefinitionPtr fdef
    )
        : function(
            std::move(fdef)
        )
        , frame(
            function->consts
        )
    {
    }

    explicit FunctionCall(
        FunctionDefinitionPtr fdef
        , LocalFrame frame_
    )
        : function(
            std::move(fdef)
        )
        , frame(
            std::move(frame_)
        )
    {
    }

    FunctionDefinition::EOperations getOperation() const {
        return function->operations[runner].operation;
    }

    bool next() noexcept {
        return ++runner < function->operations.size();
    }

    Cell popLocal() {
        auto value = std::move(
            frame.back()
        );
        frame.pop_back();
        return value;
    }

    void pushLocal(Cell cell) {
        frame.push_back(
            std::move(cell)
        );
    }

    void getConst() {
        // copy
        auto cell = function->consts[
            function->operations[runner].position
        ];
        this->pushLocal(
            std::move(cell)
        );
    }

    void getLocal() {
        // copy
        auto local = frame[
            function->operations[runner].position
        ];
        this->pushLocal(
            std::move(local)
        );
    }

    void getGlobal(const GlobalFrame& global) {
        const auto& name = function->names[
            function->operations[runner].position
        ];
        // copy
        this->pushLocal(
            global.at(name)
        );
    }

    void setGlobal(GlobalFrame& global) {
        const auto& name = function->names[
            function->operations[runner].position
        ];
        global[name] = this->popLocal();
    }

    void stackRewind() {
        frame.resize(
            frame.size() - function->operations[runner].position
        );
    }

    void skip() {
        runner += function->operations[runner].position;
    }

    LocalFrame createArgs() {
        auto size = function->operations[runner].position;
        auto newFrame = LocalFrame(size);
        while (size--) {
            newFrame[size] = popLocal();
        }
        return newFrame;
    }

private:
    LocalFrame frame;
    FunctionDefinitionPtr function;
    std::size_t runner = 0;
};

struct Environment {
    std::vector<FunctionCall> CallStack;
    std::unordered_map<TVarName, Cell> vars;

    bool isStackEmpty() {
        return this->CallStack.empty();
    }

    FunctionCall* stackTop() {
        return &this->CallStack.back();
    }

    FunctionCall* stackPush(
        FunctionCall frame
    ) {
        this->CallStack.push_back(
            std::move(frame)
        );
        return this->stackTop();
    }

    void stackPop() {
        return this->CallStack.pop_back();
    }
};

bool FunctionDefinition::step(Environment& env) {
    auto call = env.stackTop();
    switch (call->getOperation()) {
        case Nope:
            break;
        case GetConst:
            call->getConst();
            break;
        case GetLocal:
            call->getLocal();
            break;
        case GetGlobal:
            call->getGlobal(env.vars);
            break;
        case SetGlobal:
            call->setGlobal(env.vars);
            break;
        case RunFunction:
            {
                auto args = call->createArgs();
                auto fdef = boost::get<FunctionDefinitionPtr>(
                    call->popLocal()
                );
                call = env.stackPush(
                    FunctionCall(
                        std::move(fdef),
                        std::move(args)
                    )
                );
            }
            break;
        case RunExternalFunction:
            {
                auto frame = call->createArgs();
                auto fdef = boost::get<ExtFunctionPtr>(
                    call->popLocal()
                );
                fdef->call(frame);
                if (!frame.empty()) {
                    call->pushLocal(
                        std::move(frame.back())
                    );
                } else {
                    call->pushLocal(
                        Cell(Nil{})
                    );
                }
            }
            break;
        case StackRewind:
            call->stackRewind();
            break;
        case SkipIfTrue:
            {
                auto guard = call->popLocal();
                if (isNil(guard)) {
                    call->skip();
                }
            }
            break;
        case SkipIfFalse:
            {
                auto guard = call->popLocal();
                if (!isNil(guard)) {
                    call->skip();
                }
            }
            break;
    }
    if (!call->next()) {
        auto ret = call->popLocal();
        env.stackPop();
        if (env.isStackEmpty()) {
            return false;
        }
        call = env.stackTop();
        call->pushLocal(
            std::move(ret)
        );
    }
    return true;
}


/**
* Как быть с заполнением let ?
* GetFunction
* ...GetLocalVar...
* ...GetGlobalVar...
* RunFunction - результат run нужно положить в LocalScope наверх, как в стек
* когда выйдем за скобку let - stackRewind на количество переменных
*/

/**
* Что с возвращаемым значением?
* Мы его кладем в stack самым последним - так ведь?
* Выйдя из функции мы будем иметь заполненый LocalScope - в вызывающей функции можем взять столько сколько нужно аргументов.
*
* Где записать количество возвращаемых аргументов?
* position для RunFunction будет означать сколько переменных с конца LocalScope забрать себе.
* Вызываемая функция может вызывать сколько угодно значений, только вызывающая определяет сколько оттуда взять. Но всегда с конца.
* Но как она это поймет?
* Нее, пусть возвращается только последниее значение.
*/

/**
* Все не POD типы передаются по указателю. Может быть в том числе строки. ХЗ пока.
* Для передачи в качестве аргумента копии объекта необходимы специальные функции (copy x) и (deep-copy x)
*/

int main() {
    Environment env;
    env.vars.insert(
        std::make_pair(
            "+", Cell(std::make_shared<ExtSum>())
        )
    );
    env.vars.insert(
        std::make_pair(
            "print", Cell(std::make_shared<ExtPrint>())
        )
    );
    env.vars.insert(
        std::make_pair(
            "first", Cell(12)
        )
    );
    env.vars.insert(
        std::make_pair(
            "second", Cell(13)
        )
    );
    env.vars.insert(
        std::make_pair(
            "third", Cell(14)
        )
    );
    auto fdef = std::make_shared<FunctionDefinition>();
    auto frame = LocalFrame{};
    fdef->names.push_back("print");
    fdef->names.push_back("+");
    fdef->names.push_back("first");
    fdef->names.push_back("second");
    fdef->names.push_back("third");
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::GetGlobal,
            0
        )
    );
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::GetGlobal,
            1
        )
    );
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::GetGlobal,
            2
        )
    );
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::GetGlobal,
            3
        )
    );
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::RunExternalFunction,
            2
        )
    );
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::GetGlobal,
            4
        )
    );
    fdef->operations.push_back(
        FunctionDefinition::Step(
            FunctionDefinition::RunExternalFunction,
            2
        )
    );
    env.CallStack.push_back(
        FunctionCall(
            std::move(fdef),
            std::move(frame)
        )
    );
    while (FunctionDefinition::step(env)) {
    }
}
