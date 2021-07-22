#include "../include/delaunayFunctions.hpp"

HalfEdge *createEdge(Point *from, Point *to, Face *leftFace, Face *rightFace)
{
  auto edge = new HalfEdge(from, to);
  from->insertIncidentEdge(edge);

  auto twin = new HalfEdge(to, from, nullptr, nullptr, edge);
  to->insertIncidentEdge(twin);
  twin->setFace(leftFace);

  edge->setTwin(twin);
  edge->setFace(rightFace);

  return edge;
}