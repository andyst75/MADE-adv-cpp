#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <cmath>
#include <set>
#include <typeinfo>
#include <vector>

const double EPS = 1e-8;

class Point {
 public:
  Point();

  ~Point();

  Point(double x, double y);

  double x = NAN;
  double y = NAN;

  static double distance(const Point &point1, const Point &point2);
  double distance(const Point &other);
  void reflex(const Point &center);
  void rotate(const Point &center, double angle);

  friend bool operator==(const Point &left, const Point &right);
  friend bool operator!=(const Point &left, const Point &right);
};

class Line {
 private:
  Point p1;
  Point p2;
  double arc = NAN;
  double shift = NAN;

 public:
  Line(Point p1, Point p2);
  Line(double arc, double shift);
  Line(Point p, double arc);

  double distance();
  static void reflex(const Line &axis, Point &center);

  Point intersect(const Line &other);
  bool isLeft(const Point &p);

  friend bool operator==(const Line &left, const Line &right);
  friend bool operator!=(const Line &left, const Line &right);
};

class Shape {
 public:
  virtual ~Shape() = 0;

  virtual double perimeter() = 0;
  virtual double area() = 0;

  virtual size_t extclassfamily() const = 0;
  virtual bool equals(const Shape &shape) const = 0;

  friend bool operator==(const Shape &shape1, const Shape &shape2);
  friend bool operator!=(const Shape &shape1, const Shape &shape2);

  virtual void rotate(const Point &center, double angle) = 0;
  virtual void reflex(const Point &center) = 0;
  virtual void reflex(const Line &axis) = 0;
  virtual void scale(const Point &center, double coefficient) = 0;
};

class Polygon : public Shape {
 protected:
  std::vector<Point> points;

 public:
  Polygon();
  Polygon(std::vector<Point> points);
  ~Polygon(){};

  size_t extclassfamily() const override final;

  double perimeter() override;
  double sign_area();
  double area() override;
  void rotate(const Point &center, double angle) override;
  void reflex(const Point &center) override;
  void reflex(const Line &axis) override;
  void scale(const Point &center, double coefficient) override;
  bool equals(const Shape &shape) const override final;

  size_t verticesCount();
  std::vector<Point> const getVertices() const;
};

class Ellipse : public Shape {
 protected:
  Point f1, f2;
  double a = 0;

 public:
  Ellipse();
  Ellipse(Point f1, Point f2, double a);
  ~Ellipse();

  size_t extclassfamily() const override final;

  double perimeter() override;
  double area() override;
  void rotate(const Point &center, double angle) override;
  void reflex(const Point &center) override;
  void reflex(const Line &axis) override;
  void scale(const Point &center, double coefficient) override;

  bool equals(const Shape &shape) const override final;

  Point center() const;
  std::pair<Point, Point> focuses();
  double eccentricity();
};

class Circle : public Ellipse {
 public:
  Circle(Point p, double r);
  ~Circle();
  double perimeter() override;
  double area() override;
  double radius() const;
  static std::pair<Point, Point> intersect(const Circle &circle1,
                                           const Circle &circle2);
  std::pair<Point, Point> intersect(const Circle &other);
};

class Rectangle : public Polygon {
 public:
  ~Rectangle();
  Rectangle();
  Rectangle(Point p1, Point p2, double k);

  Point center();
  std::pair<Line, Line> diagonals();
};

class Triangle : public Polygon {
 public:
  ~Triangle();
  Triangle(Point p1, Point p2, Point p3);

  Circle circumscribedCircle();
  Circle inscribedCircle();
  Point centroid();
  Point orthocenter();
  Line EulerLine();
  Circle ninePointsCircle();
};

class Square : public Rectangle {
 public:
  ~Square();
  Square(Point p1, Point p2);
  Circle circumscribedCircle();
  Circle inscribedCircle();
};

#endif /* GEOMETRY_H_ */
