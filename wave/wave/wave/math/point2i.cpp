#include "point2i.h"

const point2i point2i::DIRECTIONS[] = {
    point2i(-1,  0), point2i(-1, -1), point2i( 0, -1), point2i( 1, -1),
    point2i( 1,  0), point2i( 1,  1), point2i( 0,  1), point2i(-1,  1),
    point2i( 0,  0)
};

const point2i point2i::MIN(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
const point2i point2i::MAX(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());


