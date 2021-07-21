#ifndef POINT_H
#define POINT_H

/**
 * This class represents of a point.
*/
class Point
{
public:
  Point() : x(0), y(0), id(-1) {}

  Point(int a1, int a2) : x(a1), y(a2), id(-1) {}

  Point(int a1, int a2, int idx) : x(a1), y(a2), id(idx) {}

  int getId() const { return id; }

  bool operator==(const Point &p) const
  {
    return x == p.x && y == p.y;
  }

  bool operator!=(const Point &p) const
  {
    return x != p.x && y != p.y;
  }

  bool operator>(const Point &p) const
  {
    if (y != p.y)
      return y > p.y;
    else
      return x < p.x;
  }

  bool operator<(const Point &p) const
  {
    if (y != p.y)
      return y < p.y;
    else
      return x > p.x;
  }

  bool operator>=(const Point &p) const
  {
    return *this == p || *this > p;
  }

  bool operator<=(const Point &p) const
  {
    return *this == p || *this < p;
  }

  Point &operator=(const Point &p)
  {
    x = p.x;
    y = p.y;
    id = p.getId();
    return *this;
  }

public:
  int x, y;

private:
  int id;
};

#endif // POINT_H