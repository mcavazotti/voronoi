#ifndef HALF_EDGE_H
#define HALF_EDGE_H

#include "point.hpp"

class Face;

/**
 * This class represents a node for the DCEL
*/
class HalfEdge
{
public:
  HalfEdge() : p1(nullptr), p2(nullptr), p(nullptr), n(nullptr), t(nullptr), f(nullptr) {}
  HalfEdge(PointInt *a1) : p1(a1), p2(nullptr), p(nullptr), n(nullptr), t(nullptr), f(nullptr) {}
  HalfEdge(PointInt *a1, PointInt *a2) : p1(a1), p2(a2), p(nullptr), n(nullptr), t(nullptr), f(nullptr) {}
  HalfEdge(PointInt *a1, PointInt *a2, HalfEdge *prev, HalfEdge *next, HalfEdge *twin) : p1(a1), p2(a2), p(prev), n(next), t(twin), f(nullptr) {}

  bool operator>(const HalfEdge &e) const
  {
    return (*p1) > (*(e.from()));
  }

  bool operator<(const HalfEdge &e) const
  {
    return (*p1) < (*(e.from()));
  }

  bool operator>=(const HalfEdge &e) const
  {
    return (*p1) >= (*(e.from()));
  }

  bool operator<=(const HalfEdge &e) const
  {
    return (*p1) <= (*(e.from()));
  }

  PointInt *from() const { return p1; }
  PointInt *to() const { return p2; }
  HalfEdge *prev() const { return p; }
  HalfEdge *next() const { return n; }
  HalfEdge *twin() const { return t; }
  Face *face() const { return f; }

  void setFrom(PointInt *a) { p1 = a; }
  void setTo(PointInt *a) { p2 = a; }
  void setPoints(PointInt *a1, PointInt *a) { p2 = a; }
  void setPrev(HalfEdge *e) { p = e; }
  void setNext(HalfEdge *e) { n = e; }
  void setTwin(HalfEdge *e) { t = e; }
  void setFace(Face *fa) { f = fa; }

private:
  PointInt *p1, *p2;
  HalfEdge *p, *n, *t;
  Face *f;
};

#endif