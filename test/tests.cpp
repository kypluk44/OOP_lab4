#include <gtest/gtest.h>

#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>

#include "../include/Array.h"
#include "../include/Hexagon.h"
#include "../include/Pentagon.h"
#include "../include/Rhombus.h"

namespace {

constexpr double PI = 3.14159265358979323846;

template <size_t N>
std::string regularPolygonInput(double radius, double startAngle = 0.0) {
    std::ostringstream oss;
    oss << std::setprecision(15);
    for (size_t i = 0; i < N; ++i) {
        const double angle = startAngle + 2.0 * PI * static_cast<double>(i) / static_cast<double>(N);
        const double x = radius * std::cos(angle);
        const double y = radius * std::sin(angle);
        oss << x << ' ' << y << ' ';
    }
    return oss.str();
}

template <class T>
void fillFigure(T& figure, const std::string& input) {
    std::istringstream iss(input);
    iss >> figure;
}

}  // namespace

TEST(RhombusTest, ComputesSurfaceAndCenter) {
    Rhombus<double> rhombus;
    fillFigure(rhombus, "0 0 1 1 2 0 1 -1");

    EXPECT_NEAR(double(rhombus), 2.0, 1e-6);
    const auto c = rhombus.center();
    EXPECT_NEAR(c.x, 1.0, 1e-6);
    EXPECT_NEAR(c.y, 0.0, 1e-6);
}

TEST(RhombusTest, RejectsUnequalSides) {
    Rhombus<double> rhombus;
    EXPECT_THROW(fillFigure(rhombus, "0 0 2 0 3 1 1 1"), std::invalid_argument);
}

TEST(RhombusTest, RejectsDuplicateVertices) {
    Rhombus<double> rhombus;
    EXPECT_THROW(fillFigure(rhombus, "0 0 1 1 1 1 2 0"), std::invalid_argument);
}

TEST(PentagonTest, ValidatesRegularPolygon) {
    constexpr double radius = 3.0;
    Pentagon<double> pentagon;
    fillFigure(pentagon, regularPolygonInput<5>(radius));

    const double expectedArea = 0.5 * 5.0 * radius * radius * std::sin(2.0 * PI / 5.0);
    EXPECT_NEAR(double(pentagon), expectedArea, 1e-6);
    const auto c = pentagon.center();
    EXPECT_NEAR(c.x, 0.0, 1e-6);
    EXPECT_NEAR(c.y, 0.0, 1e-6);
}

TEST(PentagonTest, RejectsVerticesWithDifferentRadius) {
    Pentagon<double> pentagon;
    const std::string invalid =
        "2 0 0 2 -2 0 0 -2 3 0";  // last vertex further from centroid
    EXPECT_THROW(fillFigure(pentagon, invalid), std::invalid_argument);
}

TEST(PentagonTest, HandlesHighPrecisionCoordinates) {
    constexpr double radius = 1.23456789;
    constexpr double startAngle = 0.321987654;
    Pentagon<double> pentagon;
    fillFigure(pentagon, regularPolygonInput<5>(radius, startAngle));

    const double expectedArea = 0.5 * 5.0 * radius * radius * std::sin(2.0 * PI / 5.0);
    EXPECT_NEAR(double(pentagon), expectedArea, 1e-9);

    const auto c = pentagon.center();
    EXPECT_NEAR(c.x, 0.0, 1e-9);
    EXPECT_NEAR(c.y, 0.0, 1e-9);
}

TEST(HexagonTest, RejectsDegeneratePolygon) {
    Hexagon<double> hexagon;
    EXPECT_THROW(fillFigure(hexagon, "0 0 1 0 2 0 3 0 4 0 5 0"), std::invalid_argument);
}

TEST(HexagonTest, ComputesSurfaceAndCenter) {
    constexpr double radius = 2.5;
    Hexagon<double> hexagon;
    fillFigure(hexagon, regularPolygonInput<6>(radius));

    const double expectedArea = 0.5 * 6.0 * radius * radius * std::sin(2.0 * PI / 6.0);
    EXPECT_NEAR(double(hexagon), expectedArea, 1e-6);

    const auto c = hexagon.center();
    EXPECT_NEAR(c.x, 0.0, 1e-6);
    EXPECT_NEAR(c.y, 0.0, 1e-6);
}

TEST(HexagonTest, HandlesHighPrecisionCoordinates) {
    constexpr double radius = 1.987654321;
    constexpr double startAngle = -0.222222222;
    Hexagon<double> hexagon;
    fillFigure(hexagon, regularPolygonInput<6>(radius, startAngle));

    const double expectedArea = 0.5 * 6.0 * radius * radius * std::sin(2.0 * PI / 6.0);
    EXPECT_NEAR(double(hexagon), expectedArea, 1e-9);

    const auto c = hexagon.center();
    EXPECT_NEAR(c.x, 0.0, 1e-9);
    EXPECT_NEAR(c.y, 0.0, 1e-9);
}

TEST(ArrayTest, CalculatesTotalSurfaceForSharedPtrFigures) {
    Array<std::shared_ptr<Figure<double>>> figures;

    auto rhombus = std::make_shared<Rhombus<double>>();
    fillFigure(*rhombus, "0 0 1 2 2 0 1 -2");
    figures.add(rhombus);

    auto hexagon = std::make_shared<Hexagon<double>>();
    fillFigure(*hexagon, regularPolygonInput<6>(2.0, 0.5));
    figures.add(hexagon);

    EXPECT_EQ(figures.getSize(), 2);
    const double expectedHexArea = 0.5 * 6.0 * 4.0 * std::sin(PI / 3.0);  // 6 * R^2 * sin(2pi/6)/2
    const double expectedTotal = double(*rhombus) + expectedHexArea;
    EXPECT_NEAR(figures.totalSurface(), expectedTotal, 1e-5);

    figures.remove(0);
    EXPECT_EQ(figures.getSize(), 1);
    EXPECT_NEAR(figures.totalSurface(), expectedHexArea, 1e-5);
}

TEST(RhombusTest, HandlesHighPrecisionCoordinates) {
    const double scale = 0.123456;
    const double dx = 0.987654;
    const double dy = -0.54321;

    const std::string input =
        "0.987654 -0.54321 "
        "1.11111 -0.296298 "
        "1.234566 -0.54321 "
        "1.11111 -0.790122";

    Rhombus<double> rhombus;
    fillFigure(rhombus, input);

    const double expectedArea = 4.0 * scale * scale;
    EXPECT_NEAR(double(rhombus), expectedArea, 1e-9);

    const auto c = rhombus.center();
    EXPECT_NEAR(c.x, dx + scale, 1e-9);
    EXPECT_NEAR(c.y, dy, 1e-9);
}

TEST(ArrayTest, AccumulatesHighPrecisionAreas) {
    Array<Pentagon<double>> pentagons;
    Pentagon<double> p1;
    fillFigure(p1, regularPolygonInput<5>(0.987654321, 0.123456789));
    Pentagon<double> p2;
    fillFigure(p2, regularPolygonInput<5>(1.234567891, 0.543210987));

    pentagons.add(p1);
    pentagons.add(p2);

    const double expectedSum = double(p1) + double(p2);
    EXPECT_NEAR(pentagons.totalSurface(), expectedSum, 1e-9);
    EXPECT_EQ(pentagons.getSize(), 2);
}

TEST(ArrayTest, ThrowsOnInvalidOperations) {
    Array<std::shared_ptr<Figure<double>>> figures;
    figures.add(std::make_shared<Rhombus<double>>());
    EXPECT_THROW(figures[1], std::out_of_range);
    EXPECT_THROW(figures.remove(5), std::out_of_range);
}
