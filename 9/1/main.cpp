#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

static constexpr double EPS = 1e-9;

struct Point {
    double x{};
    double y{};

    Point() = default;
    Point(const Point&) = default;
    Point& operator=(const Point&) = default;
    Point(Point&&) noexcept = default;
    Point& operator=(Point&&) noexcept = default;
    Point(double x, double y) : x(x), y(y) {
    }
    bool operator==(const Point& rhs) const {
        return std::abs(x - rhs.x) < EPS && std::abs(y - rhs.y) < EPS;
    }
    bool operator!=(const Point& rhs) const {
        return !(rhs == *this);
    }
};

double Direction(Point& p1, Point& p2, Point& t) {
    return ((p2.x - t.x) * (p1.y - t.y)) - ((p1.x - t.x) * (p2.y - t.y));
}

double Dist(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                     (p1.y - p2.y) * (p1.y - p2.y));
}

double ConvexHullPer(std::vector<Point>& points) {
    double per = 0.0;

    std::stable_sort(
        points.begin(), points.end(),
        [](auto const& p1, auto const& p2) { return p1.x < p2.x; });
    std::stable_sort(
        points.begin(), points.end(),
        [](auto const& p1, auto const& p2) { return p1.y < p2.y; });

    auto point = 0;
    size_t n = points.size();

    size_t fp = -1;

    while (fp != 0) {
        fp = (point + 1) % n;
        for (size_t p = 0; p < n; p++) {
            if (p == point || p == (point + 1))
                continue;
            auto dir = Direction(points[fp], points[p], points[point]);
            if (dir > 0 ||
                (std::abs(dir) < EPS && Dist(points[p], points[point]) >
                                            Dist(points[fp], points[point]))) {
                fp = p;
            }
        }
        per += Dist(points[point], points[fp]);
        point = fp;
    }

    return per;
}

int main() {
    std::vector<Point> v;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        v.emplace_back(x, y);
    }

    auto p = ConvexHullPer(v);
    std::cout.precision(12);
    std::cout << p;

    return 0;
}
