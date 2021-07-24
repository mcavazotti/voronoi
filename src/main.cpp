#include "../include/voronoi.hpp"
#include "../include/delaunay.hpp"
#include "../include/ioFunctions.hpp"
#include "../include/utils.hpp"
#include <vector>

int main()
{
  pointIntVector sites;
  readPoints(sites);
  Delaunay delaunay(sites);
  // Voronoi vor(sites);

  
  return 0;
}