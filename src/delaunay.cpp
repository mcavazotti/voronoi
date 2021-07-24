#include "../include/delaunay.hpp"
#include "../include/geometricFunctions.hpp"
#include "../include/utils.hpp"
#include "../include/ioFunctions.hpp"
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

  auto face = new Face<int>();

  auto edge = createEdgeP2P<int>(top, right, nullptr, face);
  auto tmpEdge = createEdgeP2P<int>(right, left, nullptr, face);

  // tie pointers together: first and second edges
  edge->setNext(tmpEdge);
  edge->twin()->setPrev(tmpEdge->twin());
  tmpEdge->setPrev(edge);
  tmpEdge->twin()->setNext(edge->twin());

  edge = tmpEdge;
  tmpEdge = createEdgeP2P<int>(left, top, nullptr, face);

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
  faces.insert(face);

  computationPoints.push_back(top);
  computationPoints.push_back(right);
  computationPoints.push_back(left);
}

void Delaunay::triangulate()
{
  HalfEdge<int> *edge, *tmpEdge, *newEdge1, *newEdge2;
  Face<int> *tmpFace;
  std::vector<HalfEdge<int> *> tmpEdgeVector;
  for (auto &p : points)
  {
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

      tmpFace = new Face<int>();
      setFace(newEdge1, tmpFace);
      faces.insert(tmpFace);

      newEdge2->next()->next()->setNext(newEdge2);
      newEdge2->setPrev(newEdge2->next()->next());

      tmpFace = new Face<int>();
      setFace(newEdge2, tmpFace);
      faces.insert(tmpFace);
    }
    else
    {
      tmpEdge = edge->twin();
      insertPointInEdge(p, edge);

      tmpFace = insertDiagonal(edge->prev(), edge->next());
      faces.insert(tmpFace);

      tmpFace = insertDiagonal(tmpEdge->prev(), tmpEdge->next());
      faces.insert(tmpFace);
    }

    computationPoints.push_back(p);

    // legalize edges
    tmpEdgeVector = std::vector<HalfEdge<int> *>(p->incidentEdges.begin(), p->incidentEdges.end());
    for (auto &e : tmpEdgeVector)
    {
      legalizeEdge(p, e->next());
    }
  }

  // remove bounding vertices
  PointInt *tmpPoint;
  while (!computationPoints.empty() && computationPoints.front()->getId() < 0)
  {
    tmpPoint = computationPoints.front();
    computationPoints.pop_front();
    removeVertex(tmpPoint);
  }
}

/**
 * Find which triangle contains given point using barycentric coordinates.
 * 
 * If the point is on an edge, [onEdge] is set to that edge.
 */
Face<int> *Delaunay::findTriangle(PointInt *p, HalfEdge<int> **onEdge)
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

    *onEdge = nullptr;

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
      return f;
    }
  }
  *onEdge = nullptr;
  return nullptr;
}

void Delaunay::removeVertex(PointInt *p)
{


  HalfEdge<int> *tmpEdge, *twin;
  Face<int> *discartedFace, *tmpFace;
  while (!p->incidentEdges.empty())
  {
    tmpEdge = *(p->incidentEdges.begin());
    p->incidentEdges.erase(p->incidentEdges.begin());

    twin = tmpEdge->twin();
    twin->from()->removeIncidentEdge(twin);

    tmpEdge->next()->setPrev(twin->prev());
    twin->prev()->setNext(tmpEdge->next());

    tmpEdge->prev()->setNext(twin->next());
    twin->next()->setPrev(tmpEdge->prev());

    if (tmpEdge->face() == nullptr)
    {
      tmpFace = tmpEdge->face();
      discartedFace = twin->face();
    }
    else
    {
      tmpFace = twin->face();
      discartedFace = tmpEdge->face();
    }
    setFace(tmpEdge->next(), tmpFace);

    faces.erase(discartedFace);
    delete discartedFace;
    delete tmpEdge;
    delete twin;
  }
  delete p;
}