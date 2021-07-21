#include "../include/ioFunctions.hpp"
#include <iostream>

void readPoints(std::vector<Point *> &sites)
{
  int n, x, y;
  std::cin >> n;

  for (int i = 1; i <= n; i++)
  {
    std::cin >> x >> y;
    sites.push_back(new Point(x, y, i));
  }
}