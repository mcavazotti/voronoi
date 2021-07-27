#ifndef VORONOI_H
#define VORONOI_H

#include <vector>
#include <queue>
#include <map>
#include "./point.hpp"
#include "./delaunay.hpp"
#include "./utils.hpp"
#include "./geometricFunctions.hpp"

enum class corners
{
  bottom_right = 0,
  bottom_left,
  top_left,
  top_right,
};

class Voronoi
{
public:
  Voronoi() {}
  Voronoi(Delaunay const &del);
  friend void printVoronoi(Voronoi const &vor);

private:
  void prepareVoronoi();
  void buildDiagram(Delaunay const &del);

  HalfEdge<double> *createInfiniteEdge(HalfEdge<int> *edge);
  HalfEdge<double> *createSemiInfiniteEdge(HalfEdge<int> *edge, bool reverse);
  HalfEdge<double> *createEdge(HalfEdge<int> *edge);
  // void processCoincidentCircuncenter(HalfEdge<int> *edge);

  /**
   * Find corresponding half-edge in the boundary that contains given point.
   * 
   * Since the edge chain will be transversed clockwise, corner and evaluation order must be chosen accordingly.
   * 
   * Return the inner edge segment segment.
   */
  HalfEdge<double> *findBoundingSegment(PointDouble const &p, corners corner, geo::axis axis, geo::order order);

public:
  std::vector<PointInt *> sites;
  std::vector<PointDouble *> diagramVertices;
  std::vector<Face<double> *> diagramFaces;

private:
  double boundaryMaxX, boundaryMinX, boundaryMaxY, boundaryMinY;

  // External bounding edges that begins at the bounding vertices
  std::vector<HalfEdge<double> *> cornerExternalBoundingEdge;

  std::priority_queue<PointInt *, std::vector<PointInt *>, PointPointerComparison<int>> sitesQueue;
  std::map<Face<int> *, Point<double> *> triangleCircuncenters;
  std::map<HalfEdge<int> *, HalfEdge<double> *> edgeReference;
  std::map<Face<double> *, PointInt *> siteFaceReference;
  std::map<PointInt *, Face<double> *> faceReference;
};

#endif