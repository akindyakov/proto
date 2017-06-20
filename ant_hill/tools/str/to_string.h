#pragma once

#include <tools/traits/is_printable.h>

#include <sstream>
#include <string>


namespace Tools {

template<
    typename Type
    , typename = typename std::enable_if<
        IsPrintable<Type>::value
    >::type
>
inline std::string toString(const Type& value) {
    auto out = std::ostringstream{};
    out << value;
    return out.str();
}

template<
    typename Type
    , typename = typename std::enable_if<
        !IsPrintable<Type>::value
    >::type
    , bool fake = true
>
inline std::string toString(const Type& /*value*/) {
    return "<not printable>";
}

}  // namespace Tools
