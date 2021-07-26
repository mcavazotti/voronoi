#ifndef UTILS_H
#define UTILS_H

// #include <queue>
#include <vector>
#include "point.hpp"
#include "halfEdge.hpp"
#include "face.hpp"
// #include "event.hpp"

#define EPSILON 0.0001

// class eventComparison
// {
//   bool reverse;

// public:
//   eventComparison(const bool &revparam = false)
//   {
//     reverse = revparam;
//   }
//   bool operator()(GenericEvent *a, GenericEvent *b) const
//   {
//     if (reverse)
//       return a->position > b->position;
//     else
//       return a->position < b->position;
//   }
// };

// typedef std::priority_queue<GenericEvent *, std::vector<GenericEvent *>, eventComparison> EventQueue;

/**
 * Compare if two doubles are equal considering an epsilon.
 * 
 * Code taken from https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
 */
bool compareDoubleEqual(double a, double b);

bool compareDoublePointEqual(PointDouble const &a, PointDouble const &b);


template<typename T>
class PointPointerComparison
{
  bool reverse;

public:
  PointPointerComparison<T>(const bool &revparam = false)
  {
    reverse = revparam;
  }
  bool operator()(Point<T> *a, Point<T> *b) const
  {
    if (reverse)
      return (*a) > (*b);
    else
      return (*a) < (*b);
  }
};

typedef std::vector <PointInt *> pointIntVector;
#endif