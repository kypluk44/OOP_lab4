#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "include/Array.h"
#include "include/Hexagon.h"
#include "include/Pentagon.h"
#include "include/Rhombus.h"

template <typename T>
std::string figureName(const Figure<T>& figure) {
    if (dynamic_cast<const Rhombus<T>*>(&figure)) return "Ромб";
    if (dynamic_cast<const Pentagon<T>*>(&figure)) return "Пятиугольник";
    if (dynamic_cast<const Hexagon<T>*>(&figure)) return "Шестиугольник";
    return "Неизвестная фигура";
}

int main() {
    std::cout << std::fixed << std::setprecision(2);

    // ===================================================================
    // 1. Полиморфный контейнер с умными указателями
    // ===================================================================
    Array<std::shared_ptr<Figure<double>>> figures;
    figures.add(std::make_shared<Rhombus<double>>());
    figures.add(std::make_shared<Pentagon<double>>());
    figures.add(std::make_shared<Hexagon<double>>());

    std::cout << "=== Ввод вершин для 3 полиморфных фигур ===\n";
    for (size_t i = 0; i < figures.getSize(); ++i) {
        std::cout << "\nФигура " << i << " - " << figureName(*figures[i]) << '\n';
        std::cin >> *figures[i];
    }

    std::cout << "\n=== Сохраненные фигуры и их площади ===\n";
    figures.printSurfaces();

    std::cout << "\n=== Геометрические центры ===\n";
    figures.printCenters();

    std::cout << "\nСуммарная площадь полиморфного контейнера = " << figures.totalSurface()
              << "\n";

    std::cout << "\n=== Проверка операторов ===\n";
    if (*figures[0] == *figures[1])
        std::cout << "Фигура 0 равна фигуре 1\n";
    else
        std::cout << "Фигура 0 отличается от фигуры 1\n";

    std::cout << "Площадь фигуры 0 = " << double(*figures[0]) << "\n";

    std::cout << "\n=== Демонстрация копирования и перемещения (Ромб) ===\n";
    Rhombus<double> rh1;
    std::cin >> rh1;
    std::cout << "Исходный ромб:\n" << rh1 << '\n';

    Rhombus<double> rh2 = rh1;
    std::cout << "После копирования:\n" << rh2 << '\n';

    Rhombus<double> rh3 = std::move(rh1);
    std::cout << "После перемещения:\n" << rh3 << '\n';

    std::cout << "\nУдаляем фигуру с индексом 1...\n";
    figures.remove(1);
    figures.printSurfaces();

    std::cout << "\nПопытка обратиться к фигуре 10:\n";
    try {
        std::cout << figures[10];
    } catch (const std::out_of_range& e) {
        std::cerr << "Выход за границы: " << e.what() << '\n';
    }

    // ===================================================================
    // 2. Контейнер с конкретным типом
    // ===================================================================
    std::cout << "\n=== Неполиморфный контейнер: Array<Pentagon<double>> ===\n";
    Array<Pentagon<double>> pentagons;
    pentagons.add(Pentagon<double>());
    pentagons.add(Pentagon<double>());
    pentagons.add(Pentagon<double>());

    for (size_t i = 0; i < pentagons.getSize(); ++i) {
        std::cout << "\nПятиугольник " << i << '\n';
        std::cin >> pentagons[i];
    }

    std::cout << "\nПятиугольники и их площади:\n";
    for (size_t i = 0; i < pentagons.getSize(); ++i) {
        std::cout << i << ": " << pentagons[i] << " | Площадь = " << double(pentagons[i])
                  << '\n';
    }

    std::cout << "\nЦентры пятиугольников:\n";
    for (size_t i = 0; i < pentagons.getSize(); ++i) {
        const auto c = pentagons[i].center();
        std::cout << i << ": Центр = (" << c.x << ", " << c.y << ")\n";
    }

    double totalPentagonSurface = 0.0;
    for (size_t i = 0; i < pentagons.getSize(); ++i) totalPentagonSurface += double(pentagons[i]);
    std::cout << "\nСуммарная площадь пятиугольников = " << totalPentagonSurface << '\n';

    std::cout << "\nПроверка равенства пятиугольников (0 и 1):\n";
    if (pentagons[0] == pentagons[1])
        std::cout << "Пятиугольник 0 равен пятиугольнику 1\n";
    else
        std::cout << "Пятиугольник 0 отличается от пятиугольника 1\n";

    std::cout << "\nКопирование/перемещение пятиугольника:\n";
    Pentagon<double> p1;
    std::cin >> p1;
    std::cout << "Исходный пятиугольник:\n" << p1 << '\n';

    Pentagon<double> p2 = p1;
    std::cout << "После копирования:\n" << p2 << '\n';

    Pentagon<double> p3 = std::move(p1);
    std::cout << "После перемещения:\n" << p3 << '\n';

    std::cout << "\nУдаляем пятиугольник с индексом 1...\n";
    pentagons.remove(1);
    std::cout << "Оставшиеся пятиугольники:\n";
    for (size_t i = 0; i < pentagons.getSize(); ++i) {
        std::cout << i << ": " << pentagons[i] << " | Площадь = " << double(pentagons[i])
                  << '\n';
    }

    std::cout << "Пробуем обратиться к пятиугольнику 9...\n";
    try {
        std::cout << pentagons[9];
    } catch (const std::out_of_range& e) {
        std::cerr << "Выход за границы: " << e.what() << '\n';
    }

    return 0;
}
