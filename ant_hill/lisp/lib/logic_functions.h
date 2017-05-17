#pragma once

#include "cell.h"

#include <functional>


namespace Lisp {
namespace Func {

class Not
    : public Function
{
public:
    Cell call(const Function::Args& args) const override;

};

}  // namespace Func
}  // namespace Lisp
