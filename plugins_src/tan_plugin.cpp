#include "plugin_interface.h"
#include <cmath>
#include <string>
#define M_PI 3.14157

extern "C" {

const char* plugin_name() {
    return "tan";
}

double plugin_func(double x) {
    return std::tan(x * M_PI / 180.0);
}

}
