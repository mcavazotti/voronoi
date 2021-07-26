#ifndef HALF_EDGE_H
#define HALF_EDGE_H

#include "point.hpp"

template <typename T>
class Face;

/**
 * This class represents a node for the DCEL
*/
template <typename T>
class HalfEdge
{
public:
  HalfEdge() : p1(nullptr), p2(nullptr), p(nullptr), n(nullptr), t(nullptr), f(nullptr), id(-1) {}
  HalfEdge(Point<T> *a1) : p1(a1), p2(nullptr), p(nullptr), n(nullptr), t(nullptr), f(nullptr), id(-1) {}
  HalfEdge(Point<T> *a1, Point<T> *a2) : p1(a1), p2(a2), p(nullptr), n(nullptr), t(nullptr), f(nullptr), id(-1) {}
  HalfEdge(Point<T> *a1, Point<T> *a2, HalfEdge<T> *prev, HalfEdge<T> *next, HalfEdge<T> *twin) : p1(a1), p2(a2), p(prev), n(next), t(twin), f(nullptr), id(-1) {}

  bool operator>(const HalfEdge<T> &e) const
  {
    return (*p1) > (*(e.from()));
  }

  bool operator<(const HalfEdge<T> &e) const
  {
    return (*p1) < (*(e.from()));
  }

  bool operator>=(const HalfEdge<T> &e) const
  {
    return (*p1) >= (*(e.from()));
  }

  bool operator<=(const HalfEdge<T> &e) const
  {
    return (*p1) <= (*(e.from()));
  }

  Point<T> *from() const { return p1; }
  Point<T> *to() const { return p2; }
  HalfEdge<T> *prev() const { return p; }
  HalfEdge<T> *next() const { return n; }
  HalfEdge<T> *twin() const { return t; }
  Face<T> *face() const { return f; }
  int getId() const { return id; };

  void setFrom(Point<T> *a) { p1 = a; }
  void setTo(Point<T> *a) { p2 = a; }
  void setPoints(Point<T> *a1, Point<T> *a) { p2 = a; }
  void setPrev(HalfEdge<T> *e) { p = e; }
  void setNext(HalfEdge<T> *e) { n = e; }
  void setTwin(HalfEdge<T> *e) { t = e; }
  void setFace(Face<T> *fa) { f = fa; }
  void setId(int i) { id = i; }

private:
  Point<T> *p1, *p2;
  HalfEdge<T> *p, *n, *t;
  Face<T> *f;
  int id;
};

#endif