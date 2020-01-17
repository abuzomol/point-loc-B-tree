//
// Created by muzamil on 1/14/20.
//

#include "Point.h"

double Point::getX() const { return x; }
void Point::setX(double x) { Point::x = x; }
double Point::getY() const { return y; }
void Point::setY(double y) { Point::y = y; }

ostream& operator<<(ostream& os, const Point point)
{
  os << '(' << point.getX() << ',' << point.getY() << ')';
  return os;
}