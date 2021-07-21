#ifndef EVENT_H
#define EVENT_H

#include "point.hpp"

class GenericEvent {
  public:
  GenericEvent(Point p) : position(p) {};

  virtual void handleEvent() const = 0;

  public:
  const Point position;
};

#endif