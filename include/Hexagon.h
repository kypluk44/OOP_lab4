#ifndef HEXAGON_H
#define HEXAGON_H

#include <memory>
#include <stdexcept>

#include "Figure.h"

template <IsScalar T>
class Hexagon : public Figure<T> {
public:
    Hexagon() {
        for (auto& v : vertices_) v = std::make_unique<Point<T>>();
    }

    Hexagon(const Hexagon& other) {
        for (size_t i = 0; i < kVertices; ++i)
            vertices_[i] = std::make_unique<Point<T>>(*other.vertices_[i]);
    }

    Hexagon(Hexagon&& other) noexcept {
        for (size_t i = 0; i < kVertices; ++i) vertices_[i] = std::move(other.vertices_[i]);
    }

    Hexagon& operator=(const Hexagon& other) {
        if (this == &other) return *this;
        for (size_t i = 0; i < kVertices; ++i)
            vertices_[i] = std::make_unique<Point<T>>(*other.vertices_[i]);
        return *this;
    }

    Hexagon& operator=(Hexagon&& other) noexcept {
        if (this == &other) return *this;
        for (size_t i = 0; i < kVertices; ++i) vertices_[i] = std::move(other.vertices_[i]);
        return *this;
    }

    ~Hexagon() override = default;

    void print(std::ostream& os) const override {
        for (const auto& v : vertices_) os << *v << " ";
    }

    void read(std::istream& is) override {
        if (is.rdbuf() == std::cin.rdbuf())
            std::cout << "Введите 6 вершин шестиугольника (x y):\n";

        for (auto& v : vertices_) is >> *v;
        if (!validate()) throw std::invalid_argument("Точки не образуют правильный шестиугольник");
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
        const auto* rhs = dynamic_cast<const Hexagon*>(&other);
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

        for (size_t i = 1; i < kVertices; ++i) {
            const double current = vertices_[i]->distanceTo(*vertices_[(i + 1) % kVertices]);
            if (!figure_detail::approximatelyEqual(side, current)) return false;
        }

        const auto centroid = figure_detail::centroid(vertices_);
        const double radius = centroid.distanceTo(*vertices_[0]);
        if (radius < figure_detail::kEps) return false;

        for (size_t i = 1; i < kVertices; ++i) {
            const double currentRadius = centroid.distanceTo(*vertices_[i]);
            if (!figure_detail::approximatelyEqual(radius, currentRadius)) return false;
        }

        return true;
    }

private:
    static constexpr size_t kVertices = 6;
    std::unique_ptr<Point<T>> vertices_[kVertices];
};

#endif
