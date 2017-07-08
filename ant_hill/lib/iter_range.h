#pragma once

namespace Lib {

template<
    typename IteratorType
>
class IterRange {
public:
    explicit IterRange(
        IteratorType begin
        , IteratorType end
    )
        : begin_(std::move(begin))
        , end_(std::move(end))
    {
    }

    IteratorType begin() const {
        return begin_;
    }
    IteratorType cbegin() const {
        return begin_;
    }
    IteratorType end() const {
        return end_;
    }
    IteratorType cend() const {
        return end_;
    }
    IteratorType& begin() noexcept {
        return begin_;
    }
    IteratorType& end() noexcept {
        return end_;
    }

private:
    IteratorType begin_;
    IteratorType end_;
};

template<
    typename IteratorType
>
inline auto makeIterRange(
    IteratorType begin
    , IteratorType end
) {
    return IterRange<IteratorType>(
        std::move(begin), std::move(end)
    );
}

template<
    typename TContainer
>
inline auto makeIterRange(
    TContainer& cont
) {
    return IterRange<decltype(cont.begin())>(
        cont.begin(), cont.end()
    );
}

template<typename TContainer>
inline auto makeIterRange(
    const TContainer& cont
) {
    return IterRange<decltype(cont.cbegin())>(
        cont.begin(), cont.end()
    );
}

}  // namespace Lib
