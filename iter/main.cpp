#include <chrono>
#include <iostream>
#include <limits>
#include <vector>

template<
    typename TContainerIterator
    , typename TKey
>
class EnumeratorProxy
{
public:
    using TPair = std::pair<TKey, TContainerIterator>;

    class TIterator {
    public:
        explicit TIterator(
            TContainerIterator run
            , TKey startKey
        )
            : KeyValue(startKey, run)
        {
        }

        TIterator(TIterator&&) = default;
        TIterator(const TIterator&) = default;

        TIterator& operator=(TIterator&&) = default;
        TIterator& operator=(const TIterator&) = default;

        inline TIterator& operator++() {
            ++KeyValue.first;
            ++KeyValue.second;
            return *this;
        }

        inline bool operator!=(const TIterator& other) {
            return KeyValue.second != other.KeyValue.second;
        }

        inline TPair& operator*() {
            return KeyValue;
        }

    private:
        TPair KeyValue;
    };

public:
    explicit EnumeratorProxy(
        TContainerIterator begin
        , TContainerIterator end
        , TKey start
    )
        : Begin(begin)
        , End(end)
        , Start(start)
    {
    }

    EnumeratorProxy(EnumeratorProxy&&) = default;
    EnumeratorProxy(const EnumeratorProxy&) = default;

    EnumeratorProxy& operator=(EnumeratorProxy&&) = default;
    EnumeratorProxy& operator=(const EnumeratorProxy&) = default;

    inline TIterator end() {
        return TIterator(End, std::numeric_limits<TKey>::max());
    }
    inline TIterator begin() {
        return TIterator(Begin, Start);
    }
private:
    TContainerIterator Begin;
    TContainerIterator End;
    TKey Start;
};

template<
    typename TContainerIterator
    , typename TKey = size_t
>
EnumeratorProxy<TContainerIterator, TKey> Enumerate(
    TContainerIterator begin
    , TContainerIterator end
    , TKey start = TKey{0}
) {
    return EnumeratorProxy<TContainerIterator, TKey>(begin, end, start);
}

template<
    typename TContainer
    , typename TKey = size_t
>
auto Enumerate(
    TContainer& container
    , TKey start = TKey{0}
) {
    return EnumeratorProxy<decltype(container.begin()), TKey>(
        container.begin(),
        container.end(),
        start
    );
}

int main() {
    const auto vec = std::vector<int>(9999999, 1);
    int sum = 0;
    auto rangeStart = std::chrono::high_resolution_clock::now();
    for (const auto& keyValue : Enumerate(vec)) {
        sum /= keyValue.first + 1;
        sum += *keyValue.second;
    }
    auto rangeEnd = std::chrono::high_resolution_clock::now();
    std::cerr << "sum: " << sum << std::endl;

    sum = 0;
    auto indexStart = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < vec.size(); ++i) {
        sum /= i + 1;
        sum += vec[i];
    }
    auto indexEnd = std::chrono::high_resolution_clock::now();
    std::cerr << "sum: " << sum << std::endl;


    std::cerr
        << "Range: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(rangeEnd - rangeStart).count()
        << "ns.\n";
    std::cerr
        << "Index: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(indexEnd - indexStart).count()
        << "ns.\n";
    return 0;
}
