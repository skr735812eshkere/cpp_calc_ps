#include "plugin_interface.h"
#include <cmath>

#ifndef _WIN32
    #define PLUGIN_API extern "C" __attribute__((visibility("default")))
#endif

namespace {
    constexpr double PI = 3.14159265358979323846;
    double degToRad(double degrees) {
        return degrees * PI / 180.0;
    }
}

PLUGIN_API const char* getFunctionName() {
    return "cos";
}

PLUGIN_API double calcFunction(double x) {
    if (std::isnan(x)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (std::isinf(x)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return std::cos(degToRad(x));
}
