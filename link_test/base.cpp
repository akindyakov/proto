#include "base.h"

const std::string& getText() {
    static const auto text = std::string(
        "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"
    );
    return text;
}
