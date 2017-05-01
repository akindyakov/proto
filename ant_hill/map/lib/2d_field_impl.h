namespace Map {

template<typename TCell>
Field<TCell> ScanFromText(std::istream& is) {
    // std::cerr << "scan field" << std::endl;
    auto size = Vector{0, 0};
    is >> size;
    //is >> size.Y;
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
    //auto point = minPt;
    auto field = Field<TCell>(size, pt);
    for (; pt.Y < field.max().Y; ++pt.Y) {
        for (pt.X = field.min().X; pt.X < field.max().X; ++pt.X) {
            if (!is.eof() && is.good()) {
                if (is.peek() == '\n') {
                    is.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );
                    Validate(pt.X, field.min().X);
                }
                is >> field.at(pt);
            }
        }
    }
    return field;
}

template<typename TCell>
void PrintToText(std::ostream& os, const Field<TCell>& field) {
    os << field.size() << '\n';
    os << field.min() << '\n';
    auto pt = field.min();
    for (; pt.Y < field.max().Y; ++pt.Y) {
        for (pt.X = field.min().X; pt.X < field.max().X; ++pt.X) {
            os << field.at(pt);
        }
        os << '\n';
    }
}

}  // namespace Map
