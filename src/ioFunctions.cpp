#include "../include/ioFunctions.hpp"
#include <iostream>

void readPoints(std::vector<PointInt *> &sites)
{
  int n, x, y;
  std::cin >> n;

  for (int i = 1; i <= n; i++)
  {
    std::cin >> x >> y;
    sites.push_back(new PointInt(x, y, i));
  }
}