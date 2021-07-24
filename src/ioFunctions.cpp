#include "../include/ioFunctions.hpp"
#include <iostream>
#include <stdio.h>
#include "../include/utils.hpp"

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

void printDelaunay(Delaunay const &del)
{
  std::vector<HalfEdge<int> *> edges;
  std::cout << del.computationPoints.size() << "\n";
  for (auto const &p : del.computationPoints)
  {
    std::cout << p->getId() << " " << p->x << " " << p->y << "\n";
    for (auto const &e : p->incidentEdges)
    {
      edges.push_back(e);
    }
  }

  std::cout << edges.size() << "\n";

  for (auto const &e : edges)
  {
    std::cout << e->from()->getId() << " " << e->to()->getId() << "\n";
  }
}

void delaunayDebug(Delaunay const &del)
{
  fprintf(stderr, "DEBUGING FACES\n\n");
  fprintf(stderr, "num faces: %ld\n\n", del.faces.size());
  int i = 0;
  HalfEdge<int> *tmp, *first;
  for (auto const &face : del.faces)
  {
    fprintf(stderr, "face #%d\n", i++);
    first = face->edgeChain();
    fprintf(stderr, "\tchain:\n");
    fprintf(stderr, "\t(%d,%d)", first->from()->x, first->from()->y);
    tmp = first->next();
    while (tmp != first)
    {
      fprintf(stderr, " -> (%d,%d)", tmp->from()->x, tmp->from()->y);
      tmp = tmp->next();
    }
    fprintf(stderr, "\n");
  }
}