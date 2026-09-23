
#ifndef UTILS_HPP
#define UTILS_HPP

#include <QtGlobal>

namespace Utils {
    enum class Arch {
        x64,
        x86
    };

    constexpr Arch currentArch() {
        return (sizeof(quintptr) == 8) ? Arch::x64 : Arch::x86;
    }
}

#endif