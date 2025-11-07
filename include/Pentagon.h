#ifndef PENTAGON_H
#define PENTAGON_H

#include "Figure.h"

template <IsScalar T>
class Pentagon : public RegularPolygon<T, 5> {
public:
    Pentagon() : RegularPolygon<T, 5>("pentagon") {}
    ~Pentagon() override = default;
};

#endif
