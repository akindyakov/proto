#pragma once

#include <lib/str/to_string.h>

#include <lib/exception.h>


namespace Lib {

template<
    typename ExceptionType
>
ExceptionType&& streamArgsToException(
    ExceptionType&& error
) {
    return std::forward<ExceptionType>(error);
}

template<
    typename ExceptionType
    , typename ExceptionArgType
    , typename... ExceptionArgsType
>
ExceptionType&& streamArgsToException(
    ExceptionType&& error
    , ExceptionArgType&& exceptionArg
    , ExceptionArgsType&&... exceptionArgs
) {
    return streamArgsToException(
        std::forward<ExceptionType>(error)
            << std::forward<ExceptionArgType>(exceptionArg),
        exceptionArgs...
    );
}

template<
    typename FirstT
    , typename ExceptionType = Exception
    , typename... ExceptionArgsType
>
inline void validateTrue(
    const FirstT& value
    , ExceptionType error
    , ExceptionArgsType... exceptionArgs
) {
    if (!value) {
        throw streamArgsToException(error, exceptionArgs...)
            << "[" << Lib::toString(value) << "] is suppose to be true"
        ;
    }
}

template<
    typename FirstT
    , typename SecondT
    , typename ExceptionType = Exception
    , typename... ExceptionArgsType
>
inline void validateEqual(
    const FirstT& value
    , const SecondT& other
    , ExceptionType error
    , ExceptionArgsType... exceptionArgs
) {
    if (!(value == other)) {
        throw streamArgsToException(error, exceptionArgs...)
            << "[" << Lib::toString(value)
            << "] is suppose to be equal to ["
            << Lib::toString(other)
            << "]."
        ;
    }
}

template<
    typename FirstT
    , typename SecondT
    , typename ExceptionType = Exception
    , typename... ExceptionArgsType
>
inline void validateNotEqual(
    const FirstT& value
    , const SecondT& other
    , ExceptionType error
    , ExceptionArgsType... exceptionArgs
) {
    if (value == other) {
        throw streamArgsToException(error, exceptionArgs...)
            << "[" << Lib::toString(value)
            << "] is suppose to be not equal to ["
            << Lib::toString(other)
            << "]."
        ;
    }
}

}  // namespace Lib
