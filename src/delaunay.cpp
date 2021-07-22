#include "../include/delaunay.hpp"
#include "../include/delaunayFunctions.hpp"

/**
 * Compute bounding triangle 
 */
void Delaunay::prepareTriangulation(int minX, int maxX, int minY, int maxY)
{
  int deltaX = (maxX - minX);
  int deltaY = (maxY - minY);
  int meanX = deltaX / 2;
  int meanY = deltaY / 2;

  auto top = new Point(meanX, meanY + deltaY * 10);
  auto left = new Point(meanX - deltaX * 5, meanY - deltaY * 5);
  auto right = new Point(meanX + deltaX * 5, meanY - deltaY * 5);

  auto face = new Face();

  auto edge = createEdge(top, right, nullptr, face);
  auto tmpEdge = createEdge(right, left, nullptr, face);

  // tie pointers together: first and second edges
  edge->setNext(tmpEdge);
  edge->twin()->setPrev(tmpEdge->twin());
  tmpEdge->setPrev(edge);
  tmpEdge->twin()->setNext(edge->twin());

  edge = tmpEdge;
  tmpEdge = createEdge(left, top, nullptr, face);

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
  for (auto &p : points)
  {
    auto face = findTriangle(p);
  }
  
}

Face *Delaunay::findTriangle(Point *p) {
  for (auto &f : faces)
  {
    
  }
  
}