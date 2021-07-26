#include "../include/utils.hpp"
#include <cmath>

bool compareDoubleEqual(double a, double b)
{
  return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

bool compareDoublePointEqual(PointDouble const &a, PointDouble const &b)
{
  return compareDoubleEqual(a.x, b.x) && compareDoubleEqual(a.y, b.y);
}