//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_POINT_H
#define POINT_LOC_B_TREE_POINT_H

#include <ostream>

using namespace std;

class Point
{
private:
  double x;
  double y;

public:
  // assume for now all coordinates are positives
  Point() : x(-1), y(-1) {}
  Point(double x, double y) : x(x), y(y) {}

  double getX() const;
  void setX(double x);
  double getY() const;
  void setY(double y);
};

ostream& operator<<(ostream& os, const Point point);

struct XLessThan
{
  bool operator()(const Point& a, const Point& b)
  {
    return a.getX() < b.getX();
  }
};

struct YLessThan
{
  bool operator()(const Point& a, const Point& b)
  {
    return a.getY() < b.getY();
  }
};


#endif // POINT_LOC_B_TREE_POINT_H
