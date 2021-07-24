#include "../include/delaunayFunctions.hpp"
#include <iostream>

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
  toEdge->from()->insertIncidentEdge(twin);
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

  edge->setTo(p);
  edge->twin()->setTo(p);

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
  auto twin = edge->twin();
  if (twin->face() != nullptr)
  {
    PointDouble pd = computeCircuncenter(PointDouble(*p), PointDouble(*(edge->from())), PointDouble(*(edge->to())));

    double r = computeDistance(pd, PointDouble(*p));
    double dist = computeDistance(pd, PointDouble(*(twin->next()->to())));

    if (dist < r)
    {
      // flip edge
      edge->from()->removeIncidentEdge(edge);
      edge->to()->removeIncidentEdge(twin);

      edge->next()->setPrev(twin->prev());
      twin->prev()->setNext(edge->next());

      edge->prev()->setNext(twin->next());
      twin->next()->setPrev(edge->prev());

      edge->setFrom(twin->next()->to());
      edge->from()->insertIncidentEdge(edge);
      twin->setTo(edge->from());

      twin->setFrom(edge->next()->to());
      twin->from()->insertIncidentEdge(twin);
      edge->setTo(twin->from());

      edge->setPrev(twin->next());
      edge->prev()->setNext(edge);

      twin->setPrev(edge->next());
      twin->prev()->setNext(twin);

      edge->setNext(edge->prev()->prev());
      edge->next()->setPrev(edge);

      twin->setNext(twin->prev()->prev());
      twin->next()->setPrev(twin);

      setFace(edge, edge->face());
      setFace(twin, twin->face());

      legalizeEdge(p, edge->prev());
      legalizeEdge(p, twin->next());
    }
  }
}