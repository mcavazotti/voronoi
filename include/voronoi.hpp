#ifndef VORONOI_H
#define VORONOI_H

#include <vector>
#include "./point.hpp"

class Voronoi
{
public:
  Voronoi() {}
  Voronoi(std::vector<Point *> points);

public:
  std::vector<Point *> sites;
};

#endif