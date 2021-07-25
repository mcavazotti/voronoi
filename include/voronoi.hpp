#ifndef VORONOI_H
#define VORONOI_H

#include <vector>
#include <map>
#include "./point.hpp"
#include "./delaunay.hpp"

class Voronoi
{
public:
  Voronoi() {}
  Voronoi(Delaunay const &del);

private:
  void prepareVoronoi();
  void buildDiagram(Delaunay const &del);

  void createInfiniteEdge(HalfEdge<int> *edge);
  void createSemiInfiniteEdge(HalfEdge<int> *edge);
  void createEdge(HalfEdge<int> *edge);
  void processCoincidentCircuncenter(HalfEdge<int> *edge);

public:
  std::vector<PointInt *> sites;

private:
  double boundaryMaxX, boundaryMinX, boundaryMaxY, boundaryMinY;

  std::vector<PointDouble *> vertices;
  std::map<Face<int> *, Point<double> *> triangleCircuncenters;
  std::map<HalfEdge<int> *, HalfEdge<double> *> edgeReference;
};

#endif