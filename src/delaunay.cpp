#include "../include/delaunay.hpp"
#include "../include/delaunayFunctions.hpp"
#include "../include/utils.hpp"
#include <iostream>

/**
 * Compute bounding triangle 
 */
void Delaunay::prepareTriangulation(int minX, int maxX, int minY, int maxY)
{
  int deltaX = (maxX - minX);
  int deltaY = (maxY - minY);
  int meanX = deltaX / 2;
  int meanY = deltaY / 2;
  int meanDelta = (deltaX + deltaY) / 2;

  auto top = new PointInt(meanX, meanY + meanDelta * 10);
  auto left = new PointInt(meanX - meanDelta * 5, meanY - meanDelta * 5);
  auto right = new PointInt(meanX + meanDelta * 5, meanY - meanDelta * 5);

  auto face = new Face();

  auto edge = createEdgeP2P(top, right, nullptr, face);
  auto tmpEdge = createEdgeP2P(right, left, nullptr, face);

  // tie pointers together: first and second edges
  edge->setNext(tmpEdge);
  edge->twin()->setPrev(tmpEdge->twin());
  tmpEdge->setPrev(edge);
  tmpEdge->twin()->setNext(edge->twin());

  edge = tmpEdge;
  tmpEdge = createEdgeP2P(left, top, nullptr, face);

  // tie pointers together: second and third edges
  edge->setNext(tmpEdge);
  edge->twin()->setPrev(tmpEdge->twin());
  tmpEdge->setPrev(edge);
  tmpEdge->twin()->setNext(edge->twin());

  // tie pointers together to close chain: third and first edges
  edge = edge->prev();
  edge->setPrev(tmpEdge);
  tmpEdge->setNext(edge);
  edge->twin()->setNext(tmpEdge->twin());
  tmpEdge->twin()->setPrev(edge->twin());

  // point to edge leaving topmost vertex
  face->setChain(tmpEdge->next());
  faces.push_back(face);

  computationPoints.push_back(top);
  computationPoints.push_back(right);
  computationPoints.push_back(left);
}

void Delaunay::triangulate()
{
  HalfEdge *edge, *tmpEdge, *newEdge1, *newEdge2;
  Face *tmpFace;
  edgeVector tmpEdgeVector;
  for (auto &p : points)
  {
    std::cerr << "Inserting point: (" << p->x << "," << p->y << ")\n";
    auto face = findTriangle(p, &edge);
    if (face == nullptr)
    {
      std::cerr << "Invalid point: (" << p->x << "," << p->y << ")\n";
      exit(-1);
    }

    // Point inside triangle
    if (edge == nullptr)
    {
      newEdge1 = createEdgeP2E(p, face->edgeChain());
      tmpEdge = face->edgeChain()->next();
      newEdge2 = createEdgeP2E(p, tmpEdge);

      newEdge1->setPrev(newEdge2->twin());
      newEdge2->twin()->setNext(newEdge1);
      setFace(newEdge1, face);

      newEdge1 = newEdge2;

      newEdge2 = createEdgeP2E(p, tmpEdge->next());

      newEdge1->setPrev(newEdge2->twin());
      newEdge2->twin()->setNext(newEdge1);

      tmpFace = new Face();
      setFace(newEdge1, tmpFace);
      faces.push_back(tmpFace);

      newEdge2->next()->next()->setNext(newEdge2);
      newEdge2->setPrev(newEdge2->next()->next());

      tmpFace = new Face();
      setFace(newEdge2, tmpFace);
      faces.push_back(tmpFace);
    }
    else
    {
      tmpEdge = edge->twin();
      insertPointInEdge(p, edge);

      tmpFace = insertDiagonal(edge->prev(), edge->next());
      faces.push_back(tmpFace);

      tmpFace = insertDiagonal(tmpEdge->prev(), tmpEdge->next());
      faces.push_back(tmpFace);
    }

    points.push_back(p);

    // legalize edges
    tmpEdgeVector = edgeVector(p->incidentEdges.begin(), p->incidentEdges.end());
    for (auto &e : tmpEdgeVector)
    {
      legalizeEdge(p, e->next());
    }
  }
}

/**
 * Find which triangle contains given point using barycentric coordinates.
 * 
 * If the point is on an edge, [onEdge] is set to that edge.
 */
Face *Delaunay::findTriangle(PointInt *p, HalfEdge **onEdge)
{
  double l1, l2, l3, det;
  PointInt *p1, *p2, *p3;
  for (auto &f : faces)
  {
    p1 = f->edgeChain()->from();
    p2 = f->edgeChain()->to();
    p3 = f->edgeChain()->prev()->from();

    // compute barycentric coordinates
    det = (p2->y - p3->y) * (p1->x - p3->x) + (p3->x - p2->x) * (p1->y - p3->y);

    l1 = ((p2->y - p3->y) * (p->x - p3->x) + (p3->x - p2->x) * (p->y - p3->y)) / det;
    l2 = ((p3->y - p1->y) * (p->x - p3->x) + (p1->x - p3->x) * (p->y - p3->y)) / det;
    l3 = 1 - l1 - l2;

    if (compareDoubleEqual(l1, 0.0))
    {
      *onEdge = f->edgeChain()->next();
    }
    else if (compareDoubleEqual(l2, 0.0))
    {
      *onEdge = f->edgeChain()->prev();
    }
    else if (compareDoubleEqual(l2, 0.0))
    {
      *onEdge = f->edgeChain();
    }

    if (l1 >= 0 && l2 >= 0 && l3 >= 0)
    {
      *onEdge = nullptr;
      return f;
    }
  }
  *onEdge = nullptr;
  return nullptr;
}