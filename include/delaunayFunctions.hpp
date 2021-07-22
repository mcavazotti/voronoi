#ifndef DELAUNAY_FUNC_H
#define DELAUNAY_FUNC_H

#include "delaunay.hpp"

HalfEdge *createEdge(Point *from, Point *to, Face *leftFace, Face *rightFace);

#endif