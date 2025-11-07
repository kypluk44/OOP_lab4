#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <iostream>
#include <memory>
#include <stdexcept>

#include "Figure.h"

template <IsScalar T>
class Rhombus : public Figure<T> {
public:
    Rhombus() {
        for (auto& v : vertices_) v = std::make_unique<Point<T>>();
    }

    Rhombus(const Rhombus& other) {
        for (size_t i = 0; i < kVertices; ++i)
            vertices_[i] = std::make_unique<Point<T>>(*other.vertices_[i]);
    }

    Rhombus(Rhombus&& other) noexcept {
        for (size_t i = 0; i < kVertices; ++i) vertices_[i] = std::move(other.vertices_[i]);
    }

    Rhombus& operator=(const Rhombus& other) {
        if (this == &other) return *this;
        for (size_t i = 0; i < kVertices; ++i)
            vertices_[i] = std::make_unique<Point<T>>(*other.vertices_[i]);
        return *this;
    }

    Rhombus& operator=(Rhombus&& other) noexcept {
        if (this == &other) return *this;
        for (size_t i = 0; i < kVertices; ++i) vertices_[i] = std::move(other.vertices_[i]);
        return *this;
    }

    void print(std::ostream& os) const override {
        for (const auto& v : vertices_) os << *v << " ";
    }

    void read(std::istream& is) override {
        if (is.rdbuf() == std::cin.rdbuf())
            std::cout << "Введите 4 вершины ромба (x y) по порядку:\n";

        for (auto& v : vertices_) is >> *v;
        if (!validate()) throw std::invalid_argument("Точки не образуют ромб");
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
        const auto* rhs = dynamic_cast<const Rhombus*>(&other);
        if (!rhs) return false;
        return figure_detail::sequencesEqual(vertices_, rhs->vertices_);
    }

    bool operator!=(const Figure<T>& other) const override {
        return !(*this == other);
    }

    bool validate() const override {
        if (figure_detail::hasDuplicateVertices(vertices_)) return false;
        if (figure_detail::surface(vertices_) < figure_detail::kEps) return false;

        const double side = vertices_[0]->distanceTo(*vertices_[1]);
        if (side < figure_detail::kEps) return false;
        for (size_t i = 1; i < kVertices; ++i) {
            const double current = vertices_[i]->distanceTo(*vertices_[(i + 1) % kVertices]);
            if (!figure_detail::approximatelyEqual(side, current)) return false;
        }

        const double mid1x =
            (static_cast<double>(vertices_[0]->x) + static_cast<double>(vertices_[2]->x)) / 2.0;
        const double mid1y =
            (static_cast<double>(vertices_[0]->y) + static_cast<double>(vertices_[2]->y)) / 2.0;
        const double mid2x =
            (static_cast<double>(vertices_[1]->x) + static_cast<double>(vertices_[3]->x)) / 2.0;
        const double mid2y =
            (static_cast<double>(vertices_[1]->y) + static_cast<double>(vertices_[3]->y)) / 2.0;

        return figure_detail::approximatelyEqual(mid1x, mid2x) &&
               figure_detail::approximatelyEqual(mid1y, mid2y);
    }

private:
    static constexpr size_t kVertices = 4;
    std::unique_ptr<Point<T>> vertices_[kVertices];
};

#endif
