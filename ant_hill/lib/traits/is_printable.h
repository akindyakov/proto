#pragma once

#include <ostream>
#include <type_traits>


namespace Lib {

template<
    typename T
>
class IsPrintable {
private:
    using StreamType = std::ostream&;
    static void detectType(...);

    template<
        typename A
    >
    static decltype(
        std::declval<StreamType>() << std::declval<A>()
    )
    detectType(const A&);

public:
    static constexpr bool value = std::is_same<
        std::ostream&,
        decltype(detectType(std::declval<T>()))
    >::value;
};

}  // namespace Lib
