#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>
#include "point.hpp"

class Delaunay
{
public:
  Delaunay(std::vector<Point *> const &p)
  {
    points = p;
    triangulate();
  }

public:
  std::vector<Point *> points;

private:
  void triangulate();
};
#endif