#include "map_renderer.h"

namespace map_renderer {
    bool IsZero(double value) {
        return std::abs(value) < EPSILON;
    }
}