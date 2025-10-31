#include "plugin_interface.h"
#include <cmath>
#define M_PI 3.14157


extern "C" {

const char* plugin_name() {
    return "deg";
}

double plugin_func(double x) {
    return x * 180.0 / M_PI;
}

}
