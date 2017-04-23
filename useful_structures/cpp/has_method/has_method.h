#include <utility>

// как проверить что метод (не)константен ?!

#define CREATE_HAS_METHOD(RETURN_VALUE, METHOD_NAME, ...)       \
template<typename Class>                                        \
struct has_##METHOD_NAME {                                      \
private:                                                        \
    class TFakeClass {};                                        \
                                                                \
    static TFakeClass                                           \
    Detect(...);                                                \
                                                                \
    template<typename U>                                        \
    static decltype(std::declval<U>().METHOD_NAME(__VA_ARGS__)) \
    Detect(const U&);                                           \
                                                                \
public:                                                         \
    static constexpr bool value = std::is_same<                 \
        RETURN_VALUE,                                           \
        decltype(Detect(std::declval<Class>()))                 \
    >::value;                                                   \
}                                                               \

