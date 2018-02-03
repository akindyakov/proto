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

using FunctionDefinitionPtr =    std::shared_ptr<FunctionDefinition>;
using PostponedFunctionCallPtr = std::shared_ptr<FunctionCall>;

struct Nil {};

using Cell = boost::variant<
    Nil
    , int
    , std::string
    , FunctionDefinitionPtr
    , PostponedFunctionCallPtr
>;

bool isNil(const Cell& cell) {
    return cell.which() == 0;
}

// may we use stringview here?
using TVarName = std::string;

using LocalFrame = std::vector<Cell>;
using GlobalFrame = std::unordered_map<TVarName, Cell>;

struct Environment;

struct FunctionDefinition {
    enum EOperations {
        Nope,
        //UnpackFunction,     // Берем последнее значение со стека и превращаем его в вызываемый объект со стеком. лдем указатель на функцию в стек.
        // Для любой функции: создаем новый LocalScope - кладем его в специальный стек.
        //GetLocal,          // Берем из локального скоупа по номеру в стеке.
        GetGlobal,         // Берез из глобального скоупа по имени, которое берем по номеру из локального скоупа имен.
        SetGlobal,
        RunFunction,          // Запуск функции, сколько аргументов положить в функцию, а так же где эту самую функцию взять
        RunExternalFunction,  // Запуск внешней по отношению к языку функции (встроеные и притянутые из CPP функции)
        StackRewind,    // Сколько нужно отмотать?
        SkipIfTrue,
        SkipIfFalse,
    };
    struct Step {
        EOperations operation;
        std::size_t position = 0;
    };
    std::vector<Step> operations;
    std::size_t ArgsNumber;
    std::vector<TVarName> VarNames;

    static bool step(Environment& env);
};

class FunctionCall {
public:
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

    const Cell& getLocal() const {
        return frame[
            function->operations[runner].position
        ];
    }

    void getGlobal(const GlobalFrame& global) {
        const auto& name = function->VarNames[
            function->operations[runner].position
        ];
        this->pushLocal(
            global.at(name)
        );
    }

    void setGlobal(GlobalFrame& global) {
        const auto& name = function->VarNames[
            function->operations[runner].position
        ];
        global[name] = this->popLocal();
    }

    void rewind() {
        frame.resize(
            frame.size() - function->operations[runner].position
        );
    }

    void skip() {
        runner += function->operations[runner].position;
    }

    LocalFrame createNewFrame() {
        auto size = function->operations[runner].position;
        auto newFrame = LocalFrame(size);
        while (size--) {
            newFrame[size] = popLocal();
        }
        return newFrame;
    }

private:
    void setGlobal(GlobalFrame& global, Cell cell) const {
        const auto& name = function->VarNames[
            function->operations[runner].position
        ];
        global[name] = std::move(cell);
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
        case FunctionDefinition::Nope:
            break;
        case FunctionDefinition::GetGlobal:
            call->getGlobal(env.vars);
            break;
        case FunctionDefinition::SetGlobal:
            call->setGlobal(env.vars);
            break;
        case FunctionDefinition::RunFunction:
            {
                auto frame = call->createNewFrame();
                auto fdef = boost::get<FunctionDefinitionPtr>(
                    call->popLocal()
                );
                call = env.stackPush(
                    FunctionCall(
                        std::move(fdef),
                        std::move(frame)
                    )
                );
            }
            break;
        case FunctionDefinition::RunExternalFunction:
            // TODO:
            break;
        case FunctionDefinition::StackRewind:
            call->rewind();
            break;
        case FunctionDefinition::SkipIfTrue:
            {
                auto guard = call->popLocal();
                if (isNil(guard)) {
                    call->skip();
                }
            }
            break;
        case FunctionDefinition::SkipIfFalse:
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
    while (FunctionDefinition::step(env)) {
    }
}
