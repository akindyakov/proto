#include "boost/variant.hpp"
#include <iostream>

//class my_visitor : public boost::static_visitor<int>
//{
//public:
//    int operator()(int i) const
//    {
//        return i;
//    }
//
//    int operator()(const std::string & str) const
//    {
//        return str.length();
//    }
//};

int main() {
   //boost::variant< int, std::string, void > u("hello world");
   //std::cout << u << '\n'; // output: hello world

   //int result = boost::apply_visitor( my_visitor(), u );
   //std::cout << result << '\n'; // output: 11 (i.e., length of "hello world")
    return 0;
}
