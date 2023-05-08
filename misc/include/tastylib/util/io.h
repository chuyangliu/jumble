#ifndef TASTYLIB_IO_H_
#define TASTYLIB_IO_H_

#include "tastylib/internal/base.h"
#include <iostream>

TASTYLIB_NS_BEGIN

void printLn() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
void printLn(const T& first, const Args&... rest) {
    std::cout << first;
    printLn(rest...);
}

TASTYLIB_NS_END

#endif
