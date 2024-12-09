#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
  int x;
  int y;

  bool operator<(const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }

  bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

long long Cross(const Point& o, const Point& a, const Point& b) {
  return static_cast<long long>(a.x - o.x) * (b.y - o.y) -
         static_cast<long long>(a.y - o.y) * (b.x - o.x);
}

std::vector<Point> ConvexHull(std::vector<Point>& pts) {
  size_t n = pts.size();
  size_t k = 0;
  std::vector<Point> hull(2 * n);

  std::sort(pts.begin(), pts.end());

  for (size_t i = 0; i < n; ++i) {
    while (k >= 2 && Cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) {
      k--;
    }
    hull[k++] = pts[i];
  }

  for (size_t i = n - 1, t = k + 1; i > 0; --i) {
    while (k >= t && Cross(hull[k - 2], hull[k - 1], pts[i - 1]) <= 0) {
      k--;
    }
    hull[k++] = pts[i - 1];
  }

  hull.resize(k - 1);
  return hull;
}

double PolygonArea(const std::vector<Point>& polygon) {
  long long area2 = 0;
  size_t n = polygon.size();
  for (size_t i = 0; i < n; ++i) {
    area2 += static_cast<long long>(polygon[i].x) * polygon[(i + 1) % n].y -
             static_cast<long long>(polygon[(i + 1) % n].x) * polygon[i].y;
  }
  const double cTwo = 2.0;
  return std::abs(area2) / cTwo;
}

int main() {
  int n;
  std::cin >> n;
  std::vector<Point> pts(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> pts[i].x >> pts[i].y;
  }

  std::sort(pts.begin(), pts.end());
  pts.erase(std::unique(pts.begin(), pts.end()), pts.end());

  std::vector<Point> hull = ConvexHull(pts);

  std::reverse(hull.begin(), hull.end());

  size_t start = 0;
  for (size_t i = 1; i < hull.size(); ++i) {
    if (hull[i].x < hull[start].x ||
        (hull[i].x == hull[start].x && hull[i].y < hull[start].y)) {
      start = i;
    }
  }

  std::vector<Point> result;
  size_t k = hull.size();
  for (size_t i = 0; i < k; ++i) {
    result.push_back(hull[(start + i) % k]);
  }

  double area = PolygonArea(result);

  std::cout << k << "\n";
  for (const auto& p : result) {
    std::cout << p.x << " " << p.y << "\n";
  }
  std::cout << std::fixed << std::setprecision(1) << area << "\n";

  return 0;
}
