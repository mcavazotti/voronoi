#include "../include/utils.hpp"
#include <cmath>


bool compareDoubleEqual(double a, double b)
{
  return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

