#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>


namespace Lib {

class Exception
    : public std::exception
{
private:
    std::string whatHappen;

public:
    explicit Exception() = default;

    explicit Exception(
        std::string msg
    )
        : whatHappen(std::move(msg))
    {
    }

    virtual ~Exception() = default;

    const char* what() const throw() override {
        return whatHappen.c_str();
    }

    const std::string& message() const noexcept{
        return whatHappen;
    }

    void append(std::string str) {
        whatHappen.append(str);
    }
};

/*
 * another posible implementations:
 *
 * template<
 *     typename Err
 *     , typename Val
 * , typename = typename std::enable_if<
 *     std::is_convertible<
 *         typename std::add_lvalue_reference<
 *             typename std::remove_cv<
 *                 typename std::remove_reference<Err>::type
 *             >::type
 *         >::type,
 *         std::add_lvalue_reference<ExceptionPrivate>::type
 *     >::value
 * >::type
 *
 * template<
 *     typename Err
 *     , typename Val
 *     , typename = typename std::enable_if<
 *         std::is_convertible<
 *             typename std::remove_cv<
 *                 typename std::remove_reference<Err>::type
 *             >::type&,
 *             ExceptionPrivate&
 *         >::value
 *     >::type
 * >
*/
template<
    typename Err
    , typename Val
    , typename = typename std::enable_if<
        std::is_convertible<
            typename std::decay<Err>::type&,
            Exception&
        >::value
    >::type
>
inline Err&& operator<<(
    Err&& err
    , const Val& val
) {
    std::ostringstream ostr(std::ios_base::ate);
    ostr << val;
    err.append(ostr.str());
    return std::forward<Err>(err);
}

}  // namespace Lib
