#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>


using TValue = int;

// may we use stringview here?
using TVarName = std::string;

struct GlobalScope {
    std::unordered_map<TVarName, TValue> vars;
};

struct LocalScope {
    std::size_t argsNumber;
    std::vector<TValue> stack;
    std::vector<TVarName> varNames;
};

struct FunctionDefinition {
    enum EOperations {
        GetLocalFunction,     // Где брать функцию? Из локального скопа значений по номеру. Клдем указатель на функцию в стек.
        GetGlobalFunction,    // Где брать функцию? Из глобального словаря функций по имени лежащему в локальном скоупе имен по номеру.
        // Для любой функции: создаем новый LocalScope - кладем его в специальный стек.
        GetLocalVar,          // Берем из локального скоупа по номеру в стеке.
        GetGlobalVar,         // Берез из глобального скоупа по имени, которое берем по номеру из локального скоупа имен.
        RunFunction,    // Запуск функции
        StackRewind,    // Сколько нужно отмотать?
        Return,         // Что возвращаем? По сути явно перекладывает нужное значение на вершину стека
    };
    struct Step {
        EOperations operation;
        std::size_t position = 0;
    };
    std::vector<Step> script;
};

struct FunctionCall {
    LocalScope scope;
    FunctionDefinition function;  // FIXME: use pointer here!
    std::size_t scriptRunner = 0;

    FunctionDefinition::EOperations step() const {
        return function.script[scriptRunner].operation;
    }

    TValue getLocalVar() const {
        return scope.stack[
            function.script[scriptRunner].position
        ];
    }

    TValue getGlobalVarName() const {
        return scope.varNames[
            function.script[scriptRunner].position
        ];
    }

    void rewind() {
        return scope.stack.resize(
            fscope.stack.size() - unction.script[scriptRunner].position
        );
    }

    bool next() noexcept {
        return ++scriptRunner < function.script.size();
    }
};

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
    auto global = GlobalScope{};
    auto stack = std::vector<FunctionCall>{};
    stack.push_back(
        FunctionCall{}
    );  // Define global todo list
    auto stackPtr = std::size_t{0};
    while (true) {
        auto& fCall = stack[stackPtr];
        switch (fCall.step()) {
            case GetLocalFunction:
                auto value = fCall.getLocalVar();
                break;
            case GetGlobalFunction:
                break;
            case GetLocalVar:
                break;
            case GetGlobalVar:
                break;
            case RunFunction:
                break;
            case StackRewind:
                break;
            case Return;
                break;
        }
    }
}
