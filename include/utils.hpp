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

template<typename T> Point<T> computeCircuncenter(Point<T> const &p1, Point<T> const &p2, Point<T> const &p3);

template<typename T> double computeDistance(Point<T> const &a, Point<T> const &b);

typedef std::vector <PointInt *> pointIntVector;
typedef std::vector <HalfEdge *> edgeVector;
typedef std::vector <Face *> faceVector;

#endif