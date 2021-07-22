#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>
#include <limits>
#include "point.hpp"
#include "utils.hpp"

class Delaunay
{
public:
  Delaunay(std::vector<Point *> &p)
  {
    int maxX = std::numeric_limits<int>::min();
    int minX = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();

    for (auto &point : p)
    {
      maxX = point->x > maxX ? point->x : maxX;
      minX = point->x < minX ? point->x : minX;

      maxY = point->y > maxY ? point->y : maxY;
      minY = point->y < minY ? point->x : minY;
      points.push_back(point);
    }
    prepareTriangulation(minX, maxX, minY, maxY);
    triangulate();
  }

private:
  void triangulate();

  // auxiliary methods
  void prepareTriangulation(int minX, int maxX, int minY, int maxY);
  Face *findTriangle(Point *p);

public:
  pointVector points;
  faceVector faces;

private:
  pointVector computationPoints;
};
#endif