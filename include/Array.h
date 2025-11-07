#ifndef ARRAY_H
#define ARRAY_H

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "Figure.h"

template <typename>
struct is_shared_ptr : std::false_type {};

template <typename U>
struct is_shared_ptr<std::shared_ptr<U>> : std::true_type {};

template <class T>
class Array {
public:
    Array() : size_(0), capacity_(4), data_(std::make_shared<T[]>(capacity_)) {}

    template <typename U>
    requires(!std::is_pointer_v<T> && !is_shared_ptr<T>::value)
    void add(const U& value) {
        ensureCapacity();
        data_[size_++] = value;
    }

    template <typename U>
    requires(!std::is_pointer_v<T> && !is_shared_ptr<T>::value)
    void add(U&& value) {
        ensureCapacity();
        data_[size_++] = std::forward<U>(value);
    }

    template <typename U>
    requires is_shared_ptr<T>::value
    void add(U value) {
        ensureCapacity();
        data_[size_++] = std::move(value);
    }

    void remove(size_t index) {
        if (index >= size_) throw std::out_of_range("Индекс вне диапазона");
        for (size_t i = index; i + 1 < size_; ++i) data_[i] = std::move(data_[i + 1]);
        --size_;
    }

    T& operator[](size_t index) {
        if (index >= size_) throw std::out_of_range("Индекс вне диапазона");
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) throw std::out_of_range("Индекс вне диапазона");
        return data_[index];
    }

    void printSurfaces() const {
        std::cout << std::fixed << std::setprecision(2);
        for (size_t i = 0; i < size_; ++i) {
            if constexpr (requires { double(data_[i]); }) {
                std::cout << i << ": " << data_[i]
                          << " | Площадь = " << double(data_[i]) << "\n";
            } else if constexpr (requires { double(*data_[i]); }) {
                std::cout << i << ": " << *data_[i]
                          << " | Площадь = " << double(*data_[i]) << "\n";
            }
        }
    }

    void printCenters() const {
        for (size_t i = 0; i < size_; ++i) {
            if constexpr (requires { data_[i].center(); }) {
                const auto c = data_[i].center();
                std::cout << i << ": Центр = (" << c.x << ", " << c.y << ")\n";
            } else if constexpr (requires { data_[i]->center(); }) {
                const auto c = data_[i]->center();
                std::cout << i << ": Центр = (" << c.x << ", " << c.y << ")\n";
            }
        }
    }

    double totalSurface() const {
        double sum = 0.0;
        for (size_t i = 0; i < size_; ++i) {
            if constexpr (requires { double(data_[i]); }) {
                sum += double(data_[i]);
            } else if constexpr (requires { double(*data_[i]); }) {
                sum += double(*data_[i]);
            }
        }
        return sum;
    }

    void print() const {
        for (size_t i = 0; i < size_; ++i) std::cout << "[" << i << "] " << data_[i] << "\n";
    }

    size_t getSize() const { return size_; }
    size_t getCapacity() const { return capacity_; }

private:
    size_t size_;
    size_t capacity_;
    std::shared_ptr<T[]> data_;

    void ensureCapacity() {
        if (size_ < capacity_) return;

        capacity_ *= 2;
        auto newData = std::make_shared<T[]>(capacity_);
        for (size_t i = 0; i < size_; ++i) newData[i] = std::move(data_[i]);
        data_ = std::move(newData);
    }
};

#endif
