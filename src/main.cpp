#include "../include/voronoi.hpp"
#include "../include/delaunay.hpp"
#include "../include/ioFunctions.hpp"
#include <vector>

int main() {
  std::vector<Point *>sites;
  readPoints(sites);
  Delaunay delaunay(sites);
  // Voronoi vor(sites);
    

  return 0;
}