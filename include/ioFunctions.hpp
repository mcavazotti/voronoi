#ifndef IO_FUNCTIONS_H
#define IO_FUNCTIONS_H

#include <vector>
#include "point.hpp"
#include "delaunay.hpp"
#include "voronoi.hpp"

void readPoints(std::vector<PointInt *> &sites);

void printDelaunay(Delaunay const &del);

void delaunayDebug(Delaunay const &del);

void printVoronoi(Voronoi const &vor);

#endif