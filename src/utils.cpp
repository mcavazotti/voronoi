#include "../include/utils.hpp"
#include <cmath>

template Point<int> computeCircuncenter(Point<int> const &p1, Point<int> const &p2, Point<int> const &p3);
template Point<double> computeCircuncenter(Point<double> const &p1, Point<double> const &p2, Point<double> const &p3);

template double computeDistance(Point<double> const &a, Point<double> const &b);
template double computeDistance(Point<int> const &a, Point<int> const &b);

bool compareDoubleEqual(double a, double b)
{
  return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
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