#ifndef POINT_H
#define POINT_H

#include <set>

template <typename T>
class HalfEdge;

/**
 * This class represents of a point.
*/
template <typename T>
class Point
{
public:
  Point() : x(0), y(0), id(-1) {}

  Point(T a1, T a2) : x(a1), y(a2), id(-1) {}

  Point(T a1, T a2, int idx) : x(a1), y(a2), id(idx) {}

  // Casting
  template <typename U>
  operator Point<U>()
  {
    return Point<U>(static_cast<U>(this->x), static_cast<U>(this->y), this->id);
  }

  int getId() const { return id; }


  void setId(int i) { id = i; }

  void insertIncidentEdge(HalfEdge<T> *e)
  {
    incidentEdges.insert(e);
  }
  void removeIncidentEdge(HalfEdge<T> *e)
  {
    auto tmp = incidentEdges.find(e);
    if (tmp != incidentEdges.end())
    {
      incidentEdges.erase(tmp);
    }
  }

  Point<T> operator+(const Point<T> &p)
  {
    return Point<T>(x + p.x, y + p.y);
  }

  Point<T> operator-(const Point<T> &p)
  {
    return Point<T>(x - p.x, y - p.y);
  }

  bool operator==(const Point<T> &p) const
  {
    return x == p.x && y == p.y;
  }

  bool operator!=(const Point<T> &p) const
  {
    return x != p.x && y != p.y;
  }

  bool operator>(const Point<T> &p) const
  {
    if (y != p.y)
      return y > p.y;
    else
      return x < p.x;
  }

  bool operator<(const Point<T> &p) const
  {
    if (y != p.y)
      return y < p.y;
    else
      return x > p.x;
  }

  bool operator>=(const Point<T> &p) const
  {
    return *this == p || *this > p;
  }

  bool operator<=(const Point<T> &p) const
  {
    return *this == p || *this < p;
  }

  Point<T> &operator=(const Point<T> &p)
  {
    x = p.x;
    y = p.y;
    id = p.getId();
    return *this;
  }

public:
  T x, y;
  std::set<HalfEdge<T> *> incidentEdges;

private:
  int id;
};

typedef Point<int> PointInt;
typedef Point<double> PointDouble;

#endif // POINT_H