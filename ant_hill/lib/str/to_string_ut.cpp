#include "to_string.h"

#include <lib/tests/assert.h>

#include <iostream>
#include <sstream>

class UserTypeNotPrintable{};
class UserTypePrintable{};

inline std::ostream&
    operator<<(
        std::ostream& out
        , const UserTypePrintable&
    )
{
    out << "<printable user type>";
    return out;
}

void userPrintableTypeTest() {
    std::cerr << " - userPrintableTypeTest\n";
    auto ans = Lib::toString(UserTypePrintable{});
    auto rightAns = std::string("<printable user type>");
    UT_ASSERT_EQUAL(ans, rightAns);
}

void userNotPrintableTypeTest() {
    std::cerr << " - userNotPrintableTypeTest\n";
    auto ans = Lib::toString(UserTypeNotPrintable{});
    auto rightAns = std::string("<not printable>");
    UT_ASSERT_EQUAL(ans, rightAns);
}

void builtinTypesTest() {
    std::cerr << " - builtinTypesTest\n";
    {
        auto ans = Lib::toString(16);
        auto rightAns = std::string("16");
        UT_ASSERT_EQUAL(ans, rightAns);
    }
    {
        auto ans = Lib::toString(-11);
        auto rightAns = std::string("-11");
        UT_ASSERT_EQUAL(ans, rightAns);
    }
    {
        auto ans = Lib::toString(-0.99);
        auto rightAns = std::string("-0.99");
        UT_ASSERT_EQUAL(ans, rightAns);
    }
}

int main(int argn, char** argv) {
    try {
        std::cerr << "to_string_ut:\n";
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
