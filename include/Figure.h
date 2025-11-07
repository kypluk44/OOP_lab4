#ifndef FIGURE_H
#define FIGURE_H

#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include "Point.h"

namespace figure_detail {

constexpr double kEps = 1e-6;

template <IsScalar T, size_t N>
Point<T> centroid(const std::unique_ptr<Point<T>> (&vertices)[N]) {
    double sumX = 0.0;
    double sumY = 0.0;
    for (const auto& v : vertices) {
        sumX += static_cast<double>(v->x);
        sumY += static_cast<double>(v->y);
    }
    return Point<T>(static_cast<T>(sumX / N), static_cast<T>(sumY / N));
}

template <IsScalar T, size_t N>
double surface(const std::unique_ptr<Point<T>> (&vertices)[N]) {
    double area = 0.0;
    for (size_t i = 0; i < N; ++i) {
        const auto& current = vertices[i];
        const auto& next = vertices[(i + 1) % N];
        area += static_cast<double>(current->x) * static_cast<double>(next->y) -
                static_cast<double>(current->y) * static_cast<double>(next->x);
    }
    return std::abs(area) / 2.0;
}

template <IsScalar T, size_t N>
bool hasDuplicateVertices(const std::unique_ptr<Point<T>> (&vertices)[N]) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            if (*vertices[i] == *vertices[j]) return true;
        }
    }
    return false;
}

template <IsScalar T, size_t N>
bool sequencesEqual(const std::unique_ptr<Point<T>> (&lhs)[N],
                    const std::unique_ptr<Point<T>> (&rhs)[N]) {
    for (size_t shift = 0; shift < N; ++shift) {
        bool match = true;
        for (size_t i = 0; i < N; ++i) {
            const size_t j = (i + shift) % N;
            if (lhs[i]->x != rhs[j]->x || lhs[i]->y != rhs[j]->y) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

inline bool approximatelyEqual(double lhs, double rhs) {
    return std::abs(lhs - rhs) < kEps;
}

}  // namespace figure_detail

template <IsScalar T>
class Figure {
protected:
    Figure() = default;

    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
    virtual bool validate() const = 0;

public:
    virtual ~Figure() = default;

    virtual Point<T> center() const = 0;
    virtual double surface() const = 0;

    virtual operator double() const = 0;
    virtual bool operator==(const Figure<T>& other) const = 0;
    virtual bool operator!=(const Figure<T>& other) const = 0;

    friend std::istream& operator>>(std::istream& is, Figure<T>& fig) {
        fig.read(is);
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const Figure<T>& fig) {
        fig.print(os);
        return os;
    }
};

#endif
