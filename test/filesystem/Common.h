#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <iostream>
#include "filesystem/Path.h"

using namespace std::literals::string_literals;

::std::ostream &operator<<(::std::ostream &os, const io::Path &path)
{
    return os << path.AsString().c_str();
}

#endif
