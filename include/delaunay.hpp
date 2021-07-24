#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <set>
#include <deque>
#include <limits>
#include "point.hpp"
#include "utils.hpp"

class Delaunay
{
public:
  Delaunay(std::vector<PointInt *> &p)
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

  friend void printDelaunay(Delaunay const &del);

private:
  void triangulate();

  // auxiliary methods
  void prepareTriangulation(int minX, int maxX, int minY, int maxY);
  Face *findTriangle(PointInt *p, HalfEdge **onEdge);

  /**
  * Remove vertex and rearrange DCEL around it.
  */
  void removeVertex(PointInt *p);

public:
  pointIntVector points;
  std::set<Face *> faces;

private:
  std::deque<PointInt *> computationPoints;
};
#endif