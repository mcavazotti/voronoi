#ifndef GEOMETRIC_FUNC_T
#define GEOMETRIC_FUNC_T

#include "../include/geometricFunctions.hpp"
#include <iostream>
#include <cmath>

namespace geo
{

  template <typename T>
  HalfEdge<T> *createEdgeP2P(Point<T> *from, Point<T> *to, Face<T> *leftFace, Face<T> *rightFace)
  {
    auto edge = new HalfEdge<T>(from, to);
    from->insertIncidentEdge(edge);

    auto twin = new HalfEdge<T>(to, from, nullptr, nullptr, edge);
    to->insertIncidentEdge(twin);
    twin->setFace(leftFace);

    edge->setTwin(twin);
    edge->setFace(rightFace);

    return edge;
  }

  template <typename T>
  HalfEdge<T> *createEdgeP2E(Point<T> *from, HalfEdge<T> *toEdge)
  {
    auto edge = new HalfEdge<T>(from, toEdge->from(), nullptr, toEdge, nullptr);
    from->insertIncidentEdge(edge);

    auto twin = new HalfEdge<T>(toEdge->from(), from, toEdge->prev(), nullptr, edge);
    toEdge->from()->insertIncidentEdge(twin);
    edge->setTwin(twin);

    toEdge->prev()->setNext(twin);
    toEdge->setPrev(edge);

    return edge;
  }

  template <typename T>
  void setFace(HalfEdge<T> *edgeChain, Face<T> *face)
  {
    HalfEdge<T> *tmp, *highest;

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

  template <typename T>
  void insertPointInEdge(Point<T> *p, HalfEdge<T> *edge)
  {
    HalfEdge<T> *tmp1 = new HalfEdge<T>(p, edge->to(), edge, edge->next(), nullptr);
    tmp1->setFace(edge->face());
    p->insertIncidentEdge(tmp1);
    HalfEdge<T> *tmp2 = new HalfEdge<T>(p, edge->from(), edge->twin(), edge->twin()->next(), edge);
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

  template <typename T>
  Face<T> *insertDiagonal(HalfEdge<T> *fromEdge, HalfEdge<T> *toEdge)
  {
    HalfEdge<T> *tmp;
    return insertDiagonal(fromEdge, toEdge, &tmp);
  }

  template <typename T>
  Face<T> *insertDiagonal(HalfEdge<T> *fromEdge, HalfEdge<T> *toEdge, HalfEdge<T> **newEdge)
  {
    // Create diagonal and its twin
    auto diagonal = new HalfEdge<T>(toEdge->from(), fromEdge->from(), toEdge->prev(), fromEdge, nullptr);
    *newEdge = diagonal;
    toEdge->from()->insertIncidentEdge(diagonal);
    auto diagonalTwin = new HalfEdge<T>(fromEdge->from(), toEdge->from(), fromEdge->prev(), toEdge, diagonal);
    fromEdge->from()->insertIncidentEdge(diagonalTwin);

    // Update edge chain
    diagonal->setTwin(diagonalTwin);
    fromEdge->prev()->setNext(diagonalTwin);
    toEdge->prev()->setNext(diagonal);

    toEdge->setPrev(diagonalTwin);
    fromEdge->setPrev(diagonal);

    auto face = new Face<T>();

    // Update face
    setFace(diagonal, fromEdge->face());
    setFace(diagonalTwin, face);

    return face;
  }

  template <typename T>
  void legalizeEdge(Point<T> *p, HalfEdge<T> *edge)
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

  template <typename T>
  Point<T> computeCircuncenter(Point<T> const &p1, Point<T> const &p2, Point<T> const &p3)
  {
    T d = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
    T ux = ((p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y) + (p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y) + (p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y)) / d;
    T uy = ((p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x) + (p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x) + (p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x)) / d;
    return Point<T>(ux, uy);
  }

  template <typename T>
  double computeDistance(Point<T> const &a, Point<T> const &b)
  {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }

  template <typename T>
  Point<double> computeUnitaryNormal(HalfEdge<T> const &e)
  {
    auto delta = PointDouble(*e.from() - *e.to());
    auto magnitude = computeDistance<double>(delta, PointDouble(0, 0));
    return Point<double>(-delta.y/magnitude, delta.x/magnitude);
  }
}
#endif