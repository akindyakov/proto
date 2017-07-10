#include "is_printable.h"

#include <lib/tests/assert.h>

#include <iostream>
#include <sstream>


class UserTypeNotPrintable{};
class UserTypePrintable{};
std::ostream& operator<<(std::ostream& out, const UserTypePrintable&);

void userPrintableTypeTest() {
    std::cerr << " - userPrintableTypeTest\n";
    static_assert(Lib::IsPrintable<UserTypePrintable>::value, "must be printable");
}

void userNotPrintableTypeTest() {
    std::cerr << " - userNotPrintableTypeTest\n";
    static_assert(!Lib::IsPrintable<UserTypeNotPrintable>::value, "must not be printable");
}

void builtinTypesTest() {
    std::cerr << " - builtinTypesTest\n";
    static_assert(Lib::IsPrintable<short>::value, "<short> must be printable");
    static_assert(Lib::IsPrintable<int>::value, "<int> must be printable");
    static_assert(Lib::IsPrintable<long>::value, "<long> must be printable");
    static_assert(Lib::IsPrintable<long long>::value, "<long long> must be printable");
    static_assert(Lib::IsPrintable<char>::value, "<char> must be printable");
    static_assert(Lib::IsPrintable<float>::value, "<float> must be printable");
    static_assert(Lib::IsPrintable<double>::value, "<double> must be printable");
}

int main(int argn, char** argv) {
    try {
        std::cerr << "is_printable_ut:\n";
        userPrintableTypeTest();
        userNotPrintableTypeTest();
        builtinTypesTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
