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

template <IsScalar T, size_t N>
class RegularPolygon : public Figure<T> {
public:
    explicit RegularPolygon(std::string_view name) : name_(name) {
        static_assert(N >= 3, "У многоугольника должно быть не менее 3 вершин");
        for (auto& v : vertices_) v = std::make_unique<Point<T>>();
    }

    RegularPolygon(const RegularPolygon& other) : name_(other.name_) {
        for (size_t i = 0; i < N; ++i)
            vertices_[i] = std::make_unique<Point<T>>(*other.vertices_[i]);
    }

    RegularPolygon(RegularPolygon&& other) noexcept : name_(std::move(other.name_)) {
        for (size_t i = 0; i < N; ++i) vertices_[i] = std::move(other.vertices_[i]);
    }

    RegularPolygon& operator=(const RegularPolygon& other) {
        if (this == &other) return *this;
        name_ = other.name_;
        for (size_t i = 0; i < N; ++i)
            vertices_[i] = std::make_unique<Point<T>>(*other.vertices_[i]);
        return *this;
    }

    RegularPolygon& operator=(RegularPolygon&& other) noexcept {
        if (this == &other) return *this;
        name_ = std::move(other.name_);
        for (size_t i = 0; i < N; ++i) vertices_[i] = std::move(other.vertices_[i]);
        return *this;
    }

    void print(std::ostream& os) const override {
        for (const auto& v : vertices_) os << *v << " ";
    }

    void read(std::istream& is) override {
        if (is.rdbuf() == std::cin.rdbuf())
            std::cout << "Введите " << N << " вершин для фигуры " << name_ << " (x y):\n";

        for (auto& v : vertices_) is >> *v;
        if (!validate())
            throw std::invalid_argument("Переданные точки не образуют правильную фигуру " +
                                        std::string{name_});
    }

    Point<T> center() const override {
        return figure_detail::centroid(vertices_);
    }

    double surface() const override {
        return figure_detail::surface(vertices_);
    }

    operator double() const override {
        return surface();
    }

    bool operator==(const Figure<T>& other) const override {
        const auto* rhs = dynamic_cast<const RegularPolygon<T, N>*>(&other);
        if (!rhs) return false;
        return figure_detail::sequencesEqual(vertices_, rhs->vertices_);
    }

    bool operator!=(const Figure<T>& other) const override {
        return !(*this == other);
    }

    bool validate() const override {
        if (figure_detail::hasDuplicateVertices(vertices_)) return false;
        const double area = figure_detail::surface(vertices_);
        if (area < figure_detail::kEps) return false;

        const double side = vertices_[0]->distanceTo(*vertices_[1]);
        if (side < figure_detail::kEps) return false;

        for (size_t i = 1; i < N; ++i) {
            const double current = vertices_[i]->distanceTo(*vertices_[(i + 1) % N]);
            if (!figure_detail::approximatelyEqual(side, current)) return false;
        }

        const auto centroid = figure_detail::centroid(vertices_);
        const double radius = centroid.distanceTo(*vertices_[0]);
        if (radius < figure_detail::kEps) return false;

        for (size_t i = 1; i < N; ++i) {
            const double currentRadius = centroid.distanceTo(*vertices_[i]);
            if (!figure_detail::approximatelyEqual(radius, currentRadius)) return false;
        }

        return true;
    }

protected:
    std::string name_;
    std::unique_ptr<Point<T>> vertices_[N];
};

#endif
