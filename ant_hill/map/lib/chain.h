#pragma once

#include <vector>

namespace Map {

template<
    typename DirT
    , typename ValueT
>
struct ChainNode
{
    using DirType = DirT;
    using ValueType = ValueT;

    constexpr ChainNode() = default;
    constexpr ChainNode(
        DirType from_
        , ValueType value_
    ) noexcept
        : from(from_)
        , value(value_)
    {
    }

    DirType from;
    ValueType value;
};

template<
    typename DirT
    , typename ValueT
>
using Chain = std::vector<ChainNode<DirT, ValueT>>;

}  // namespace Map
