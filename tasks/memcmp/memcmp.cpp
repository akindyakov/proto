#include <iostream>
#include <cstring>

namespace NImpl {

int memcmp(const void *s1, const void *s2, std::size_t n) {
    auto pt1 = static_cast<const unsigned char*>(s1);
    auto pt2 = static_cast<const unsigned char*>(s2);
    while (n-- && *pt1 == *pt2);
    return
        n == 0u
        ? 0
        : static_cast<int>(*pt1) - static_cast<int>(*pt2)
    ;
}

}

int main() {
    {
        auto v1 = 100;
        auto v2 = v1;
        auto etalon = std::memcmp(&v1, &v2, sizeof(v1));
        auto ans = NImpl::memcmp(&v1, &v2, sizeof(v1));
        if (ans != etalon) {
            std::cerr << "0: failed " << ans << " != " << etalon << '\n';
            return 1;
        }
    }
    {
        auto pt = reinterpret_cast<const void*>(&NImpl::memcmp);
        auto etalon = std::memcmp(pt, pt, 0u);
        auto ans =  NImpl::memcmp(pt, pt, 0u);
        if (ans != etalon) {
            std::cerr << "1: failed " << ans << " != " << etalon << '\n';
            return 1;
        }
    }
    {
        auto v = std::string{
            "The sign of the result is the sign of the difference between the values of the first pair of bytes (both interpreted as unsigned char) that differ in the objects being compared."
        };
        auto etalon = std::memcmp(&v[0], &v[0], v.size());
        auto ans =  NImpl::memcmp(&v[0], &v[0], v.size());
        if (ans != etalon) {
            std::cerr << "2: failed " << ans << " != " << etalon << '\n';
            return 1;
        }
    }
    {
        auto v1 = std::string{"The"};
        auto v2 = std::string{"Thf"};
        auto etalon = std::memcmp(&v1[0], &v1[0], v1.size());
        auto ans =  NImpl::memcmp(&v2[0], &v2[0], v1.size());
        if (ans != etalon) {
            std::cerr << "3: failed " << ans << " != " << etalon << '\n';
            return 1;
        }
    }
    {
        auto v1 = std::string{"The"};
        auto v2 = std::string{"Tha"};
        auto etalon = std::memcmp(&v1[0], &v1[0], v1.size());
        auto ans =  NImpl::memcmp(&v2[0], &v2[0], v1.size());
        if (ans != etalon) {
            std::cerr << "3: failed " << ans << " != " << etalon << '\n';
            return 1;
        }
    }
    return 0;
}
