#ifndef HEXAGON_H
#define HEXAGON_H

#include "Figure.h"

template <IsScalar T>
class Hexagon : public RegularPolygon<T, 6> {
public:
    Hexagon() : RegularPolygon<T, 6>("hexagon") {}
    ~Hexagon() override = default;
};

#endif
