#include "plugin_interface.h"
#include <cmath>
#include <string>
#define M_PI 3.14157


extern "C" {

const char* plugin_name() {
    return "cot";
}

double plugin_func(double x) {
    double radians = x * M_PI / 180.0;
    return std::cos(radians) / std::sin(radians);
}

}
