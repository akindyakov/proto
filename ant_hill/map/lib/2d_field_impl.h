namespace Map {

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
