#ifndef POINT_H
#define POINT_H

#include <concepts>
#include <cmath>
#include <iostream>
#include <type_traits>

template <typename T>
concept IsScalar = std::is_scalar_v<T>;

template <IsScalar T>
class Point {
public:
    T x{0};
    T y{0};

    Point() = default;
    Point(T px, T py) : x(px), y(py) {}

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point operator/(double value) const {
        return Point(static_cast<T>(x / value), static_cast<T>(y / value));
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    double distanceTo(const Point& other) const {
        const double dx = static_cast<double>(x) - static_cast<double>(other.x);
        const double dy = static_cast<double>(y) - static_cast<double>(other.y);
        return std::sqrt(dx * dx + dy * dy);
    }

    double dot(const Point& other) const {
        return static_cast<double>(x) * static_cast<double>(other.x) +
               static_cast<double>(y) * static_cast<double>(other.y);
    }

    double cross(const Point& other) const {
        return static_cast<double>(x) * static_cast<double>(other.y) -
               static_cast<double>(y) * static_cast<double>(other.x);
    }

    friend std::istream& operator>>(std::istream& is, Point<T>& p) {
        return is >> p.x >> p.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

#endif
