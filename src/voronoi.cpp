#include "../include/voronoi.hpp"
#include "../include/utils.hpp"
#include "../include/geometricFunctions.hpp"
#include <limits>

Voronoi::Voronoi(Delaunay const &del)
{
  int maxX = std::numeric_limits<int>::min();
  int minX = std::numeric_limits<int>::max();
  int maxY = std::numeric_limits<int>::min();
  int minY = std::numeric_limits<int>::max();

  for (auto const &p : del.points)
  {
    maxX = p->x > maxX ? p->x : maxX;
    minX = p->x < minX ? p->x : minX;

    maxY = p->y > maxY ? p->y : maxY;
    minY = p->y < minY ? p->x : minY;
    sites.push_back(p);
  }

  boundaryMaxX = maxX + 5;
  boundaryMinX = minX - 5;
  boundaryMaxY = maxY + 5;
  boundaryMinY = minY - 5;

  prepareVoronoi();
  buildDiagram(del);
}

/**
 * Build Voronoi diagram
 * 
 * It follows this algorithm:
 * 
 * 1. For each triangle T in the Delaunay triangulation do:
 *    1.1 Compute circuncenter and store it in a map indexed by the face.
 *        If it is close enough to a neighbor's circuncenter, store neighbor's instead
 * 2. For each site S in the Delaunay triangulation do:
 *    2.1 For each edge E incient do S do:
 *        2.1.1 Create an edge Ev for the diagram according to local conditions (neighboring faces and edges)
 *        
 */
void Voronoi::buildDiagram(Delaunay const &del)
{
  std::vector<HalfEdge<int> *> triangleEdges;
  // Compute circuncenter and store it in a map indexed by the face.
  if (!del.faces.empty())
  {
    for (auto const &f : del.faces)
    {
      triangleEdges.clear();
      triangleEdges.push_back(f->edgeChain());
      triangleEdges.push_back(f->edgeChain()->next());
      triangleEdges.push_back(f->edgeChain()->prev());

      auto circuncenter = geo::computeCircuncenter(PointDouble(*triangleEdges[0]->from()), PointDouble(*triangleEdges[0]->from()), PointDouble(*triangleEdges[0]->from()));

      PointDouble *circuncenterPtr = nullptr;

      // check for the existence of a equivalent circuncenter in the neighborhood
      for (auto const &e : triangleEdges)
      {
        if (e->twin()->face() != nullptr)
        {
          auto neighborTrianle = e->twin()->face();
          // check if neighbor's circuncenter hs been already computated
          if (triangleCircuncenters.count(neighborTrianle) > 0)
          {
            // check if circuncenters are close enough to be the same
            if (compareDoubleEqual(geo::computeDistance(circuncenter, *triangleCircuncenters[neighborTrianle]), 0.0))
              circuncenterPtr = triangleCircuncenters[neighborTrianle];
          }
        }
      }

      if (circuncenterPtr == nullptr)
      {
        circuncenterPtr = new PointDouble(circuncenter.x, circuncenter.y);
      }

      // insert circuncenter in the data structures
      triangleCircuncenters[f] = circuncenterPtr;
      vertices.push_back(circuncenterPtr);
    }
  }

  // create edges
  for (auto const &p : del.points)
  {
    for (auto const &e : p->incidentEdges)
    {
      if (e->face() == nullptr && e->twin()->face() == nullptr)
      {
        createInfiniteEdge(e);
      }
      else
      {
        if (e->face() == nullptr)
          createSemiInfiniteEdge(e);
        else if (e->twin() == nullptr)
          createSemiInfiniteEdge(e->twin());
        else
        {
          if (triangleCircuncenters[e->face()] == triangleCircuncenters[e->twin()->face()])
          {
            processCoincidentCircuncenter(e);
          }
          else
          {
            createEdge(e);
          }
        }
      }
    }
  }
}

void Voronoi::prepareVoronoi()
{
  vertices.push_back(new PointDouble(boundaryMaxX, boundaryMaxY));
  vertices.push_back(new PointDouble(boundaryMaxX, boundaryMinY));
  vertices.push_back(new PointDouble(boundaryMinX, boundaryMinY));
  vertices.push_back(new PointDouble(boundaryMinX, boundaryMaxY));

  auto face = new Face<double>();
  HalfEdge<double> *tmpEdge;
  HalfEdge<double> *prevEdge = nullptr;
  HalfEdge<double> *firstEdge = nullptr;

  for (size_t i = 1; i < vertices.size(); i++)
  {
    tmpEdge = geo::createEdgeP2P<double>(vertices[i - 1], vertices[i], nullptr, face);

    if (firstEdge == nullptr)
    {
      firstEdge = tmpEdge;
    }

    if (prevEdge != nullptr)
    {
      tmpEdge->setPrev(prevEdge);
      tmpEdge->twin()->setNext(prevEdge->twin());

      prevEdge->setNext(tmpEdge);
      prevEdge->twin()->setPrev(tmpEdge->twin());
    }
  }

  tmpEdge->setNext(firstEdge);
  tmpEdge->twin()->setPrev(firstEdge->twin());

  firstEdge->setPrev(tmpEdge);
  firstEdge->twin()->setNext(tmpEdge->twin());
}

void Voronoi::createInfiniteEdge(HalfEdge<int> *edge)
{
  auto normal = PointDouble(geo::computeNormal(*edge));
  auto point = PointDouble(double(edge->from()->x + edge->to()->x) / 2.0, double(edge->from()->y + edge->to()->y) / 2.0);

  double t;

  if (!compareDoubleEqual(normal.y, 0.0))
  {
    // check against top boundary
    t = (boundaryMaxY - point.y) / normal.y;

    if ((point.x + normal.x * t) >= boundaryMinX && (point.x + normal.x * t) <= boundaryMaxX)
    {
      /* create edge */
    }

    // check against bottom boundary
    t = (boundaryMinY - point.y) / normal.y;

    if ((point.x + normal.x * t) >= boundaryMinX && (point.x + normal.x * t) <= boundaryMaxX)
    {
      /* create edge */
    }
  }

  if (!compareDoubleEqual(normal.y, 0.0))
  {
    // check against left boundary
    t = (boundaryMinX - point.x) / normal.x;

    if ((point.y + normal.y * t) >= boundaryMinY && (point.y + normal.y * t) <= boundaryMaxY)
    {
      /* create edge */
    }

    // check against right boundary
    t = (boundaryMaxX - point.x) / normal.x;

    if ((point.y + normal.y * t) >= boundaryMinY && (point.y + normal.y * t) <= boundaryMaxY)
    {
      /* create edge */
    }
  }
}