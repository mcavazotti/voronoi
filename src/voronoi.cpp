#include "../include/voronoi.hpp"
#include "../include/utils.hpp"
#include "../include/geometricFunctions.hpp"
#include <limits>
#include <iostream>

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
    minY = p->y < minY ? p->y : minY;
    sites.push_back(p);
    sitesQueue.push(p);
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
 * 2. For each site S in the Delaunay triangulation  from top to bottom do:
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

      auto circuncenter = geo::computeCircuncenter(PointDouble(*triangleEdges[0]->from()), PointDouble(*triangleEdges[1]->from()), PointDouble(*triangleEdges[2]->from()));

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
        diagramVertices.push_back(circuncenterPtr);
      }

      // insert circuncenter in the data structures
      triangleCircuncenters[f] = circuncenterPtr;
    }
  }

  HalfEdge<double> *newEdge;
  // create edges
  do
  {
    auto p = sitesQueue.top();
    sitesQueue.pop();

    for (auto const &e : p->incidentEdges)
    {
      if (edgeReference.count(e) <= 0)
      {
        if (e->face() == nullptr && e->twin()->face() == nullptr)
        {
          newEdge = createInfiniteEdge(e);
        }
        else
        {
          if (e->face() == nullptr)
            newEdge = createSemiInfiniteEdge(e, false);
          else if (e->twin()->face() == nullptr)
            newEdge = createSemiInfiniteEdge(e->twin(), true);
          else
          {
            // if (triangleCircuncenters[e->face()] == triangleCircuncenters[e->twin()->face()])
            // {
            // processCoincidentCircuncenter(e);
            // createEdge(e);
            // }
            // else
            // {
            createEdge(e);
            // }
          }
        }
      }
    }
  } while (!sitesQueue.empty());

  for (auto &p : sites)
  {
    for (auto &e : p->incidentEdges)
    {
      auto ref = edgeReference[e];
      auto f = ref->face();
      siteFaceReference[f] = p;
    }
  }
}

void Voronoi::prepareVoronoi()
{
  diagramVertices.push_back(new PointDouble(boundaryMaxX, boundaryMaxY));
  diagramVertices.push_back(new PointDouble(boundaryMaxX, boundaryMinY));
  diagramVertices.push_back(new PointDouble(boundaryMinX, boundaryMinY));
  diagramVertices.push_back(new PointDouble(boundaryMinX, boundaryMaxY));

  auto face = new Face<double>();

  HalfEdge<double> *tmpEdge;
  HalfEdge<double> *prevEdge = nullptr;
  HalfEdge<double> *firstEdge = nullptr;

  for (size_t i = 1; i <= diagramVertices.size(); i++)
  {
    tmpEdge = geo::createEdgeP2P<double>(diagramVertices[i - 1], diagramVertices[i % diagramVertices.size()], nullptr, face);
    cornerExternalBoundingEdge.push_back(tmpEdge->twin());

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
    prevEdge = tmpEdge;
  }

  tmpEdge->setNext(firstEdge);
  tmpEdge->twin()->setPrev(firstEdge->twin());

  firstEdge->setPrev(tmpEdge);
  firstEdge->twin()->setNext(tmpEdge->twin());

  geo::setFace(firstEdge, face);
  diagramFaces.push_back(face);

  for (auto &s : sites)
  {
    faceReference[s] = face;
  }
}

HalfEdge<double> *Voronoi::findBoundingSegment(PointDouble const &p, corners corner, geo::axis axis, geo::order order)
{
  double limitValue, fromCoord, toCoord;
  double relevantPointCoord = axis == geo::axis::horizontal ? p.x : p.y;
  auto tmpEdge = cornerExternalBoundingEdge[int(corner)];

  if (axis == geo::axis::horizontal)
  {
    if (order == geo::order::ascending)
      limitValue = boundaryMaxX;
    else
      limitValue = boundaryMinX;
  }
  else
  {
    if (order == geo::order::ascending)
      limitValue = boundaryMaxY;
    else
      limitValue = boundaryMinY;
  }
  do
  {
    fromCoord = axis == geo::axis::horizontal ? tmpEdge->from()->x : tmpEdge->from()->y;
    toCoord = axis == geo::axis::horizontal ? tmpEdge->prev()->from()->x : tmpEdge->prev()->from()->y;

    if (compareDoubleEqual(fromCoord, relevantPointCoord))
    {
      return tmpEdge->prev()->twin();
    }
    if (order == geo::order::ascending ? relevantPointCoord < toCoord : relevantPointCoord > toCoord)
    {
      return tmpEdge->prev()->twin();
    }
    tmpEdge = tmpEdge->prev();

  } while (order == geo::order::ascending ? fromCoord < limitValue : fromCoord > limitValue);

  return nullptr;
}

struct _segmentRecord
{
  double t;
  HalfEdge<double> *edge;
};

HalfEdge<double> *Voronoi::createInfiniteEdge(HalfEdge<int> *edge)
{
  auto normal = geo::computeUnitaryNormal(*edge);
  auto point = PointDouble(double(edge->from()->x + edge->to()->x) / 2.0, double(edge->from()->y + edge->to()->y) / 2.0);

  std::vector<_segmentRecord> pointsOnBoundary;

  HalfEdge<double> *tmpEdge;
  PointDouble *tmpPoint;

  double t;

  if (!compareDoubleEqual(normal.y, 0.0))
  {
    // check against top boundary
    t = (boundaryMaxY - point.y) / normal.y;
    auto xVal = (point.x + normal.x * t);

    // x_min <= x < x_max
    if (compareDoubleEqual(xVal, boundaryMinX) || (xVal > boundaryMinX && xVal < boundaryMaxX))
    {
      tmpPoint = new PointDouble(xVal, boundaryMaxY);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::top_left, geo::axis::horizontal, geo::order::ascending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointsOnBoundary.push_back({t, tmpEdge});
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointsOnBoundary.push_back({t, tmpEdge->next()});
        }
      }
    }

    // check against bottom boundary
    t = (boundaryMinY - point.y) / normal.y;
    xVal = (point.x + normal.x * t);

    // x_min < x <= x_max
    if (compareDoubleEqual(xVal, boundaryMaxX) || (xVal > boundaryMinX && xVal < boundaryMaxX))
    {
      tmpPoint = new PointDouble(xVal, boundaryMinY);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::bottom_right, geo::axis::horizontal, geo::order::descending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointsOnBoundary.push_back({t, tmpEdge});
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointsOnBoundary.push_back({t, tmpEdge->next()});
        }
      }
    }
  }

  if (!compareDoubleEqual(normal.y, 0.0))
  {
    // check against left boundary
    t = (boundaryMinX - point.x) / normal.x;
    auto yVal = (point.y + normal.y * t);

    // y_min <= y < y_max
    if (compareDoubleEqual(yVal, boundaryMinY) || (yVal > boundaryMinY && yVal < boundaryMaxY))
    {
      tmpPoint = new PointDouble(boundaryMinX, yVal);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::bottom_left, geo::axis::vertical, geo::order::ascending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointsOnBoundary.push_back({t, tmpEdge});
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointsOnBoundary.push_back({t, tmpEdge->next()});
        }
      }
    }

    // check against right boundary
    t = (boundaryMaxX - point.x) / normal.x;
    yVal = (point.y + normal.y * t);

    // y_min < y <= y_max
    if (compareDoubleEqual(yVal, boundaryMaxY) || (yVal >= boundaryMinY && yVal <= boundaryMaxY))
    {
      tmpPoint = new PointDouble(boundaryMaxX, yVal);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::top_right, geo::axis::vertical, geo::order::descending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointsOnBoundary.push_back({t, tmpEdge});
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointsOnBoundary.push_back({t, tmpEdge->next()});
        }
      }
    }
  }

  if (pointsOnBoundary.size() != 2)
  {
    std::cerr << "Calculations failed\n";
    exit(-1);
  }

  HalfEdge<double> *newEdge;
  auto newFace = geo::insertDiagonal(pointsOnBoundary[0].edge, pointsOnBoundary[1].edge, &newEdge, true);
  diagramFaces.push_back(newFace);

  if (pointsOnBoundary[0].t > pointsOnBoundary[1].t)
  {
    edgeReference[edge] = newEdge;
    edgeReference[edge->twin()] = newEdge->twin();

    faceReference[edge->from()] = newEdge->face();
    faceReference[edge->to()] = newEdge->twin()->face();
    return newEdge;
  }
  else
  {
    edgeReference[edge] = newEdge->twin();
    edgeReference[edge->twin()] = newEdge;

    faceReference[edge->from()] = newEdge->twin()->face();
    faceReference[edge->to()] = newEdge->face();
    return newEdge->twin();
  }
}

HalfEdge<double> *Voronoi::createSemiInfiniteEdge(HalfEdge<int> *edge, bool reverse)
{
  auto normal = geo::computeUnitaryNormal(*edge);
  auto point = PointDouble(double(edge->from()->x + edge->to()->x) / 2.0, double(edge->from()->y + edge->to()->y) / 2.0);

  HalfEdge<double> *pointOnBoundary;
  HalfEdge<double> *tmpEdge, *neighboorEdge;
  PointDouble *tmpPoint;

  double t;

  if (!compareDoubleEqual(normal.y, 0.0))
  {
    // check against top boundary
    t = (boundaryMaxY - point.y) / normal.y;
    auto xVal = (point.x + normal.x * t);

    // x_min <= x < x_max
    if (t > 0 && (compareDoubleEqual(xVal, boundaryMinX) || (xVal > boundaryMinX && xVal < boundaryMaxX)))
    {
      tmpPoint = new PointDouble(xVal, boundaryMaxY);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::top_left, geo::axis::horizontal, geo::order::ascending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointOnBoundary = tmpEdge;
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointOnBoundary = tmpEdge->next();
        }
      }
    }

    // check against bottom boundary
    t = (boundaryMinY - point.y) / normal.y;
    xVal = (point.x + normal.x * t);

    // x_min < x <= x_max
    if (t > 0 && (compareDoubleEqual(xVal, boundaryMaxX) || (xVal > boundaryMinX && xVal < boundaryMaxX)))
    {
      tmpPoint = new PointDouble(xVal, boundaryMinY);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::bottom_right, geo::axis::horizontal, geo::order::descending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointOnBoundary = tmpEdge;
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointOnBoundary = tmpEdge->next();
        }
      }
    }
  }

  if (!compareDoubleEqual(normal.x, 0.0))
  {
    // check against left boundary
    t = (boundaryMinX - point.x) / normal.x;
    auto yVal = (point.y + normal.y * t);

    // y_min <= y < y_max
    if (t > 0 && (compareDoubleEqual(yVal, boundaryMinY) || (yVal > boundaryMinY && yVal < boundaryMaxY)))
    {
      tmpPoint = new PointDouble(boundaryMinX, yVal);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::bottom_left, geo::axis::vertical, geo::order::ascending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointOnBoundary = tmpEdge;
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointOnBoundary = tmpEdge->next();
        }
      }
    }

    // check against right boundary
    t = (boundaryMaxX - point.x) / normal.x;
    yVal = (point.y + normal.y * t);

    // y_min < y <= y_max
    if (t > 0 && (compareDoubleEqual(yVal, boundaryMaxY) || (yVal >= boundaryMinY && yVal <= boundaryMaxY)))
    {
      tmpPoint = new PointDouble(boundaryMaxX, yVal);
      tmpEdge = findBoundingSegment(*tmpPoint, corners::top_right, geo::axis::vertical, geo::order::descending);
      if (tmpEdge == nullptr)
      {
        std::cerr << "Calculations failed\n";
        exit(-1);
      }
      else
      {
        if (compareDoublePointEqual(*tmpEdge->from(), *tmpPoint))
        {
          delete tmpPoint;
          pointOnBoundary = tmpEdge;
        }
        else
        {
          diagramVertices.push_back(tmpPoint);
          geo::insertPointInEdge(tmpPoint, tmpEdge);
          pointOnBoundary = tmpEdge->next();
        }
      }
    }
  }

  auto neighboorEdgeInt = !reverse ? edge->twin()->next() : edge->twin()->prev()->twin();
  Face<double> *newFace, *oldFace;
  HalfEdge<double> *newEdge;
  if (edgeReference.count(neighboorEdgeInt) > 0)
  {
    oldFace = pointOnBoundary->face();
    neighboorEdge = edgeReference[neighboorEdgeInt];
    tmpEdge = !reverse ? neighboorEdge->next() : neighboorEdge;
    newFace = geo::insertDiagonal(tmpEdge, pointOnBoundary, &newEdge, true);
    // If the cell is open, then the old face was overwritten.
    if (newEdge->face() != oldFace)
    {
      geo::setFace(newEdge, oldFace);
      delete newFace;
    }
    else
    {
      diagramFaces.push_back(newFace);
      faceReference[edge->to()] = newEdge->face();
      faceReference[edge->from()] = newEdge->twin()->face();
    }
    newEdge = newEdge->twin();
  }
  else
  {
    auto circuncenter = triangleCircuncenters[edge->twin()->face()];
    newEdge = geo::createEdgeP2E(circuncenter, pointOnBoundary);
  }

  edgeReference[edge] = newEdge;
  edgeReference[edge->twin()] = newEdge->twin();

  return newEdge;
}

HalfEdge<double> *Voronoi::createEdge(HalfEdge<int> *edge)
{
  HalfEdge<int> *leftNeighboorInt, *rightNeighboorInt;
  HalfEdge<double> *leftNeighboor, *rightNeighboor, *newEdge;
  Face<double> *newFace, *oldFace;

  leftNeighboorInt = edge->twin()->next();
  if (edgeReference.count(leftNeighboorInt) > 0)
    leftNeighboor = edgeReference[leftNeighboorInt];
  else
    leftNeighboor = nullptr;

  rightNeighboorInt = edge->prev()->twin();
  if (edgeReference.count(rightNeighboorInt) > 0)
    rightNeighboor = edgeReference[rightNeighboorInt];
  else
    rightNeighboor = nullptr;

  if (leftNeighboor != nullptr && rightNeighboor != nullptr)
  {
    oldFace = rightNeighboor->face();
    newFace = geo::insertDiagonal(leftNeighboor->next(), rightNeighboor, &newEdge, true);

    if (newEdge->face() != oldFace)
    {
      geo::setFace(newEdge, oldFace);
      delete newFace;
    }
    else
    {
      faceReference[edge->to()] = newEdge->face();
      faceReference[edge->from()] = newEdge->twin()->face();
      diagramFaces.push_back(newFace);
    }
    newEdge = newEdge->twin();
  }
  else
  {
    auto circ1 = triangleCircuncenters[edge->twin()->face()];
    auto circ2 = triangleCircuncenters[edge->face()];
    if (leftNeighboor == nullptr && rightNeighboor == nullptr)
    {
      auto f = faceReference[edge->from()];
      newEdge = geo::createEdgeP2P(circ1, circ2, f, f);
    }
    else
    {
      if (leftNeighboor == nullptr)
      {
        newEdge = geo::createEdgeP2E(circ1, rightNeighboor);
      }
      else
      {
        newEdge = geo::createEdgeP2E(circ2, leftNeighboor->next());
        newEdge = newEdge->twin();
      }
    }
  }

  edgeReference[edge] = newEdge;
  edgeReference[edge->twin()] = newEdge->twin();

  return newEdge;
}