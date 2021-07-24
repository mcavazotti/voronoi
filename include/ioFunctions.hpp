#ifndef IO_FUNCTIONS_H
#define IO_FUNCTIONS_H

#include <vector>
#include "point.hpp"
#include "delaunay.hpp"

void readPoints(std::vector<PointInt *> &sites);

void printDelaunay(Delaunay const &del);

void delaunayDebug(Delaunay const &del);

#endif