#include <cmath>
#include <iostream>
#include <set>
#include <typeinfo>
#include <vector>

#include "geometry.h"

/**
 * Point
 */
Point::Point(double x, double y) : x(x), y(y){};

Point::Point(){};
Point::~Point(){};

double Point::distance(const Point &point1, const Point &point2) {
  return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

double Point::distance(const Point &other) {
  return Point::distance(*this, other);
}

void Point::reflex(const Point &center) {
  x = center.x * 2 - x;
  y = center.y * 2 - y;
}

void Point::rotate(const Point &center, double angle) {
  angle *= -M_PI / 180;  // angle -> radiant
  double cosine = cos(angle);
  double sine = sin(angle);

  double new_x =
      center.x + (x - center.x) * cosine + (center.y - y) * sine + 0.5;
  double new_y =
      center.y + (x - center.x) * sine + (y - center.y) * cosine + 0.5;

  x = new_x;
  y = new_y;
}

bool operator==(const Point &left, const Point &right) {
  return (fabs(left.x - right.x) <= EPS) && (fabs(left.y - right.y) <= EPS);
}

bool operator!=(const Point &left, const Point &right) {
  return !(left == right);
}

bool operator<(const Point &left, const Point &right) {
  return right.x - left.x >= EPS ||
         (fabs(left.x - right.x) < EPS && left.y < right.y);
}

/**
 * Line
 */
Line::Line(Point p1, Point p2) : p1(p1), p2(p2) {
  arc = (p2.y - p1.y) / (p2.x - p1.x);
  shift = p1.x * p2.y - p2.x * p1.y;
};

Line::Line(double arc, double shift) : arc(arc), shift(shift){};

Line::Line(Point p, double arc) : p1(p), arc(arc) {
  shift = p1.y - arc * p1.x;
};

double Line::distance() {
  if (std::isnan(p2.x))
    return NAN;
  else
    return p1.distance(p2);
}

void Line::reflex(const Line &axis, Point &p) {
  double new_x = ((1 - axis.arc * axis.arc) * p.x + 2 * axis.arc * p.y -
                  2 * axis.arc * axis.shift) /
                 (axis.arc * axis.arc + 1);
  double new_y =
      ((axis.arc * axis.arc - 1) * p.y + 2 * axis.arc * p.x + 2 * axis.shift) /
      (axis.arc * axis.arc + 1);
  p.x = new_x;
  p.y = new_y;
}

bool operator==(const Line &left, const Line &right) {
  if (!std::isnan(left.p2.x) && !std::isnan(left.p2.x)) {
    return (left.p1 == right.p1 && left.p2 == right.p2) ||
           (left.p1 == right.p2 && left.p2 == right.p1);
  } else if ((!std::isnan(left.p2.x) && std::isnan(left.p2.x)) ||
             (std::isnan(left.p2.x) && !std::isnan(left.p2.x))) {
    return false;
  } else {
    return (left.arc == right.arc) && (left.shift == right.shift);
  }
}

bool operator!=(const Line &left, const Line &right) {
  return !(left == right);
}

Point Line::intersect(const Line &other) {
  Point p;
  if (fabs(arc - other.arc) > EPS) {
    if (fabs(arc) < EPS) {
      p.y = shift;
      p.x = (shift - other.shift) / other.arc;
    } else if (fabs(other.arc) < EPS) {
      p.y = other.shift;
      p.x = (other.shift - shift) / arc;
    } else {
      p.x = (shift - other.shift) / (arc - other.arc);
      p.y = arc * p.x + shift;
    }
  }
  return p;
}

bool Line::isLeft(const Point &p) { return p.x * arc + shift - p.y > 0; }

/**
 * Shape
 */
Shape::~Shape(){};

bool operator==(const Shape &shape1, const Shape &shape2) {
  if (shape1.extclassfamily() != shape2.extclassfamily()) {
    return false;
  }
  return shape1.equals(shape2);
}

bool operator!=(const Shape &shape1, const Shape &shape2) {
  return !(shape1 == shape2);
}

/**
 * Polygon
 */
Polygon::Polygon() {}
Polygon::Polygon(std::vector<Point> points) : points(points) {}

size_t Polygon::extclassfamily() const { return typeid(this).hash_code(); }

double Polygon::perimeter() {
  double result = 0;
  if (points.size() > 1) {
    Point p1 = points[0];
    for (size_t i = 1; i <= points.size(); ++i) {
      Point p2 = i < points.size() ? points[i] : points[0];
      result += p1.distance(p2);
      p1 = p2;
    }
  }
  return result;
}
double Polygon::sign_area() {
  double a = 0;
  if (points.size() > 1) {
    Point p1 = points[0];
    for (size_t i = 1; i <= points.size(); ++i) {
      Point p2 = i < points.size() ? points[i] : points[0];
      a += (p1.x + p2.x) * (p2.y - p1.y);
      p1 = p2;
    }
  }
  return a / 2;
}

double Polygon::area() { return std::fabs(sign_area()); }

size_t Polygon::verticesCount() { return points.size(); }

std::vector<Point> const Polygon::getVertices() const { return points; }

void Polygon::rotate(const Point &center, double angle) {
  for (Point &p : points) {
    p.rotate(center, angle);
  }
}
void Polygon::reflex(const Point &center) {
  for (Point &p : points) {
    p.reflex(center);
  }
}
void Polygon::reflex(const Line &axis) {
  for (Point &p : points) {
    Line::reflex(axis, p);
  }
}
void Polygon::scale(const Point &center, double coefficient) {
  for (auto &p : points) {
    p.x = coefficient * (p.x - center.x) + center.x;
    p.y = coefficient * (p.y - center.y) + center.y;
  }
}

bool Polygon::equals(const Shape &other) const {
  const Polygon *p = dynamic_cast<const Polygon *>(&other);

  return points.size() == p->points.size() &&
         std::set<Point>(points.begin(), points.end()) ==
             std::set<Point>(p->points.begin(), p->points.end());
}

/**
 * Triangle
 */
Triangle::~Triangle() {}

Triangle::Triangle(Point p1, Point p2, Point p3) { points = {p1, p2, p3}; };

Circle Triangle::circumscribedCircle() {
  auto a = points[0], b = points[1], c = points[2];
  double s = area();
  double r = a.distance(b) * a.distance(c) * b.distance(c) / 4 / s;
  double d = (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) * 2;

  double x = ((a.x * a.x + a.y * a.y) * (b.y - c.y) +
              (b.x * b.x + b.y * b.y) * (c.y - a.y) +
              (c.x * c.x + c.y * c.y) * (a.y - b.y)) /
             d;

  double y = ((a.x * a.x + a.y * a.y) * (c.x - b.x) +
              (b.x * b.x + b.y * b.y) * (a.x - c.x) +
              (c.x * c.x + c.y * c.y) * (b.x - a.x)) /
             d;

  return Circle(Point(x, y), r);
}

Circle Triangle::inscribedCircle() {
  Point a = points[0], b = points[1], c = points[2];
  double p = perimeter();
  double s = area();

  double r = s * 2 / p;

  double x =
      (b.distance(c) * a.x + a.distance(c) * b.x + a.distance(b) * c.x) / p;
  double y =
      (b.distance(c) * a.y + a.distance(c) * b.y + a.distance(b) * c.y) / p;

  return Circle(Point(x, y), r);
}

Point Triangle::centroid() {
  return Point((points[0].x + points[1].x + points[2].x) / 3,
               (points[0].y + points[1].y + points[2].y) / 3);
}

Point Triangle::orthocenter() {
  Point a = points[0], b = points[1], c = points[2];

  if (b.y == c.y) {
    a = points[1];
    b = points[0];
  }

  double x, y;
  double k1 = (b.x - c.x) / (c.y - b.y);
  double k2 = (a.x - b.x) / (b.y - a.y);
  x = (k1 * a.x - k2 * c.x + c.y - a.y) / (k1 - k2);
  y = (x - a.x) * k1 + a.y;

  return Point(x, y);
}

Line Triangle::EulerLine() {
  return Line(centroid(), ninePointsCircle().center());
}

Circle Triangle::ninePointsCircle() {
  Point a = points[0], b = points[1], c = points[2];

  Point p1, p2, p3;
  {
    double k1 = (b.x - c.x) / (c.y - b.y);
    double k2 = (c.y - b.y) / (c.x - b.x);
    double x, y;
    if (fabs(c.y - b.y) >= EPS) {
      x = (a.x * k1 - b.x * k2 - a.y + b.y) / (k1 - k2);
      y = (x - a.x) * k1 + a.y;
    } else {
      x = a.x;
      y = -a.y;
    }
    p1 = {x, y};
  }

  {
    double k1 = (a.x - c.x) / (c.y - a.y);
    double k2 = (c.y - a.y) / (c.x - a.x);
    double x, y;
    if (fabs(c.y - a.y) >= EPS) {
      x = (b.x * k1 - a.x * k2 + a.y - b.y) / (k1 - k2);
      y = (x - b.x) * k1 + b.y;
    } else {
      x = b.x;
      y = -b.y;
    }
    p2 = {x, y};
  }

  {
    double k1 = (a.x - b.x) / (b.y - a.y);
    double k2 = (b.y - a.y) / (b.x - a.x);
    double x, y;
    if (fabs(b.y - a.y) >= EPS) {
      x = (c.x * k1 - a.x * k2 - c.y + a.y) / (k1 - k2);
      y = (x - c.x) * k1 + c.y;
    } else {
      x = c.x;
      y = -c.y;
    }
    p3 = {x, y};
  }

  Triangle t({p1, p2, p3});
  return t.circumscribedCircle();
}

/**
 * Rectangle
 */
Rectangle::~Rectangle() {}

Rectangle::Rectangle() {}

Rectangle::Rectangle(Point p1, Point p2, double k) {
  double x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;

  Line l(p1, p2);
  double d = l.distance();
  double r1 = sqrt(d * d / (1 + k * k));
  double r2 = k * sqrt(d * d / (1 + k * k));

  Point p3, p4;
  auto is1 = Circle({x1, y1}, r1).intersect({{x2, y2}, r2});
  auto is2 = Circle({x1, y1}, r2).intersect({{x2, y2}, r1});

  if (l.isLeft(is1.first)) {
    p3 = is1.first;
    p4 = is2.second;
  } else {
    p3 = is1.second;
    p4 = is2.first;
  }

  points = {p1, p3, p2, p4};
}

Point Rectangle::center() {
  return Point((points[0].x + points[2].x) / 2,
               (points[0].y + points[2].y) / 2);
}

std::pair<Line, Line> Rectangle::diagonals() {
  return {Line(points[0], points[2]), Line(points[1], points[3])};
}

/**
 * Square
 */
Square::~Square() {}

Square::Square(Point p1, Point p2) {
  Rectangle rect(p1, p2, 1);
  points = rect.getVertices();
}

Circle Square::circumscribedCircle() {
  return Circle(center(), sqrt(2) * points[0].distance(points[1]) / 4);
}

Circle Square::inscribedCircle() {
  return Circle(center(), points[0].distance(points[1]) / 2);
}

/**
 * Ellipse
 */
Ellipse::Ellipse(){};

Ellipse::Ellipse(Point f1, Point f2, double a) : f1(f1), f2(f2), a(a){};

Ellipse::~Ellipse(){};

size_t Ellipse::extclassfamily() const { return typeid(this).hash_code(); }

double Ellipse::perimeter() { return 2 * a * 1.34050538; }

double Ellipse::area() {
  double c = Point::distance(f1, f2) / 2;
  double a = this->a / 2;
  double b = a * sqrt(1 - c * c / a / a);
  return M_PI * a * b;
}

void Ellipse::rotate(const Point &center, double angle) {
  f1.rotate(center, angle);
  f2.rotate(center, angle);
}
void Ellipse::reflex(const Point &center) {
  f1.reflex(center);
  f2.reflex(center);
}
void Ellipse::reflex(const Line &axis) {
  Line::reflex(axis, f1);
  Line::reflex(axis, f2);
}
void Ellipse::scale(const Point &center, double coefficient) {
  f1.x = coefficient * (f1.x - center.x) + center.x;
  f1.y = coefficient * (f1.y - center.y) + center.y;
  f2.x = coefficient * (f2.x - center.x) + center.x;
  f2.y = coefficient * (f2.y - center.y) + center.y;
  a *= coefficient;
}

bool Ellipse::equals(const Shape &other) const {
  const Ellipse *p = dynamic_cast<const Ellipse *>(&other);
  return (fabs(Point::distance(f1, f2) - Point::distance(p->f1, p->f2)) <
          EPS) &&
         (fabs(a - p->a) < EPS);
}

Point Ellipse::center() const { return {(f1.x + f2.x) / 2, (f1.y + f2.y) / 2}; }

std::pair<Point, Point> Ellipse::focuses() { return {f1, f2}; }

double Ellipse::eccentricity() {
  double c = Point::distance(f1, f2) / 2;
  return c / (a / 2);
}

/**
 * Circle
 */
Circle::Circle(Point p, double r) {
  f1 = p;
  f2 = p;
  a = 2 * r;  // diameter
}

Circle::~Circle() {}

double Circle::perimeter() { return M_PI * a; }

double Circle::area() { return M_PI * a * a / 4; }

double Circle::radius() const { return a / 2; }

std::pair<Point, Point> Circle::intersect(const Circle &circle1,
                                          const Circle &circle2) {
  Point p1, p2;
  Point c1 = circle1.center(), c2 = circle2.center();
  double r1 = circle1.radius(), r2 = circle2.radius();
  double d = c1.distance(c2);

  if (d > EPS && std::fabs(d - r1 - r2) > EPS && d - std::fabs(r1 - r2) > EPS) {
    double dx = (c2.x - c1.x) / d;
    double dy = (c2.y - c1.y) / d;

    double a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    double px = c1.x + a * dx;
    double py = c1.y + a * dy;

    double h = std::sqrt(r1 * r1 - a * a);

    p1 = {px + h * dy, py - h * dx};
    p2 = {px - h * dy, py + h * dx};
  }

  return {p1, p2};
}

std::pair<Point, Point> Circle::intersect(const Circle &other) {
  return Circle::intersect(*this, other);
}
