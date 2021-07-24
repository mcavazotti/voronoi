#ifndef VORONOI_H
#define VORONOI_H

#include <vector>
#include "./point.hpp"

class Voronoi
{
public:
  Voronoi() {}
  Voronoi(std::vector<PointInt *> points);

public:
  std::vector<PointInt *> sites;
};

#endif