#include "../include/delaunayFunctions.hpp"

HalfEdge *createEdgeP2P(PointInt *from, PointInt *to, Face *leftFace, Face *rightFace)
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

HalfEdge *createEdgeP2E(PointInt *from, HalfEdge *toEdge)
{
  auto edge = new HalfEdge(from, toEdge->from(), nullptr, toEdge, nullptr);
  from->insertIncidentEdge(edge);

  auto twin = new HalfEdge(toEdge->from(), from, toEdge->prev(), nullptr, edge);
  toEdge->from()->insertIncidentEdge(edge);
  edge->setTwin(twin);

  toEdge->prev()->setNext(twin);
  toEdge->setPrev(edge);

  return edge;
}

void setFace(HalfEdge *edgeChain, Face *face)
{
  HalfEdge *tmp, *highest;

  tmp = edgeChain;
  highest = edgeChain;
  do
  {
    tmp->setFace(face);
    if ((*tmp) > (*highest))
      highest = tmp;

    tmp = tmp->next();
  } while (tmp != edgeChain);
  if (face != nullptr)
    face->setChain(highest);
}

void insertPointInEdge(PointInt *p, HalfEdge *edge)
{
  HalfEdge *tmp1 = new HalfEdge(p, edge->to(), edge, edge->next(), nullptr);
  tmp1->setFace(edge->face());
  p->insertIncidentEdge(tmp1);
  HalfEdge *tmp2 = new HalfEdge(p, edge->from(), edge->twin(), edge->twin()->next(), edge);
  tmp2->setFace(edge->twin()->face());
  p->insertIncidentEdge(tmp2);

  tmp1->setTwin(edge->twin());
  tmp2->setTwin(edge);

  edge->twin()->next()->setPrev(tmp2);
  edge->next()->setPrev(tmp1);

  edge->twin()->setNext(tmp2);
  edge->twin()->setTwin(tmp1);

  edge->setNext(tmp1);
  edge->setTwin(tmp2);
}

Face *insertDiagonal(HalfEdge *fromEdge, HalfEdge *toEdge)
{
  // Create diagonal and its twin
  auto diagonal = new HalfEdge(toEdge->from(), fromEdge->from(), toEdge->prev(), fromEdge, nullptr);
  toEdge->from()->insertIncidentEdge(diagonal);
  auto diagonalTwin = new HalfEdge(fromEdge->from(), toEdge->from(), fromEdge->prev(), toEdge, diagonal);
  fromEdge->from()->insertIncidentEdge(diagonalTwin);

  // Update edge chain
  diagonal->setTwin(diagonalTwin);
  fromEdge->prev()->setNext(diagonalTwin);
  toEdge->prev()->setNext(diagonal);

  toEdge->setPrev(diagonalTwin);
  fromEdge->setPrev(diagonal);

  auto face = new Face();

  // Update face
  setFace(diagonal, fromEdge->face());
  setFace(diagonalTwin, face);

  return face;
}

void legalizeEdge(PointInt *p, HalfEdge *edge)
{
  if (edge->twin()->face() != nullptr)
  {
    PointDouble pd = computeCircuncenter(PointDouble(*p), PointDouble(*(edge->from())), PointDouble(*(edge->to())));

    double r = computeDistance(pd, PointDouble(*p));
    double dist = computeDistance(pd, PointDouble(*(edge->twin()->next()->to())));

    if(dist < r) {
      /* flip edge */
    }

  }
}