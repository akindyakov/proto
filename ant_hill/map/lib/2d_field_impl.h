#pragma once

#include <limits>


namespace Map {

template<
    typename TCell
>
void Field<TCell>::resize(
      Vector newSize
    , Vector shift
    , const CellType initCellValue
) {
    auto oldCube = this->size().cube();
    if (newSize.cube() > oldCube) {
        this->field_.resize(newSize.cube(), initCellValue);
    }

    auto oldIter = this->begin();  // create iter by old area-square
    this->area_ = Square(newSize, this->area_.min() - shift);  // renew area-square

    auto fromTo = std::map<size_t, size_t>{};
    while (oldIter.isValid()) {
        const auto& point = oldIter.point();
        if (this->inRange(point)) {
            auto oldIndex = static_cast<size_t>(signedIndexByPoint(point, oldIter.square()));
            auto newIndex = static_cast<size_t>(signedIndexByPoint(point, this->area_));
            fromTo.emplace(oldIndex, newIndex);
        }
        ++oldIter;
    }

    // only old cell(index) can be root
    auto roots = std::vector<bool>(oldCube, true);
    // remove cell that is not root (has input endges)
    for (const auto& it : fromTo) {
        if (it.second < oldCube) {
            roots[it.second] = false;
        }
    }

    for (size_t root = 0; root < roots.size(); ++root) {
        if (roots[root]) {
            // from the root to the end of chain we will go
            auto next = root;
            auto swapVal = initCellValue;
            while (true) {
                using std::swap;
                swap(
                    this->field_[next],
                    swapVal
                );
                auto it = fromTo.find(next);
                if (it != fromTo.end()) {
                    next = it->second;
                } else {
                    break;
                }
            }
        }
    }

    if (newSize.cube() < oldCube) {
        this->field_.resize(newSize.cube(), initCellValue);
    }
}

template<typename TCell>
Field<TCell> ScanFromText(std::istream& is) {
    auto size = Vector{0, 0};
    is >> size;
    is.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
    auto pt = Point{0, 0};
    is >> pt;
    is.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
    auto field = Field<TCell>(size, pt);
    for (auto iter = field.begin(); iter.isValid(); ++iter) {
        if (!is.eof() && is.good()) {
            if (is.peek() == '\n') {
                is.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );
                Lib::validateEqual(
                    iter.point().X,
                    field.min().X,
                    Exception() << "Unexpected end of line character"
                );
            }
            is >> field.at(iter.point());
        }
    }
    return field;
}

template<typename TCell>
void PrintToText(std::ostream& os, const Field<TCell>& field) {
    os << field.size() << '\n';
    os << field.min() << '\n';
    auto row = field.min().Y;
    for (auto iter = field.begin(); iter.isValid(); ++iter) {
        if (row != iter.point().Y) {
            row = iter.point().Y;
            os << '\n';
        }
        os << field.at(iter.point());
    }
    os << '\n';
}

}  // namespace Map
