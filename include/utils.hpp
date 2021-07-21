#ifndef UTILS_H
#define UTILS_H

#include <queue>
#include <vector>
#include "event.hpp"

class eventComparison
{
  bool reverse;

public:
  eventComparison(const bool &revparam = false)
  {
    reverse = revparam;
  }
  bool operator()(GenericEvent *a, GenericEvent *b) const
  {
    if (reverse)
      return a->position > b->position;
    else
      return a->position < b->position;
  }
};

typedef std::priority_queue<GenericEvent *, std::vector<GenericEvent *>, eventComparison> EventQueue;

typedef std::vector <Point *> pointVector;

#endif