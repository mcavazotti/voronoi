#include "../include/ioFunctions.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>

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

bool _sortPointsById(PointDouble *a, PointDouble *b)
{
  return a->getId() < b->getId();
}

bool _sortEdgesById(HalfEdge<double> *a, HalfEdge<double> *b)
{
  return a->getId() < b->getId();
}

bool _sortFacesById(Face<double> *a, Face<double> *b)
{
  return a->getId() < b->getId();
}

void printVoronoi(Voronoi const &vor)
{
  int edgeCount = 0;
  int vertexCount = 0;
  int faceCount = 0;

  std::vector<PointDouble *> pointsVector;
  std::vector<HalfEdge<double> *> edgesVector;
  std::vector<Face<double> *> facesVector;

  for (auto &p : vor.diagramVertices)
  {
    p->setId(++vertexCount);
    pointsVector.push_back(p);
    for (auto &e : p->incidentEdges)
    {
      e->setId(++edgeCount);
      edgesVector.push_back(e);
    }
  }
  for (auto &f : vor.diagramFaces)
  {
    f->setId(++faceCount);
    facesVector.push_back(f);
  }

  std::sort(pointsVector.begin(), pointsVector.end(), _sortPointsById);
  std::sort(edgesVector.begin(), edgesVector.end(), _sortEdgesById);
  std::sort(facesVector.begin(), facesVector.end(), _sortFacesById);

  std::cout << pointsVector.size() << " " << edgesVector.size() / 2 << " " << facesVector.size() << "\n";

  for (auto &p : pointsVector)
    std::cout << p->x << " " << p->y << " " << (*p->incidentEdges.begin())->getId() << "\n";
  for (auto &f : facesVector)
    std::cout << vor.siteFaceReference.at(f)->x << " " << vor.siteFaceReference.at(f)->y << " " << f->edgeChain()->getId() << "\n";
  for (auto &e : edgesVector)
  {
    std::cout << e->from()->getId() << " ";
    std::cout << e->twin()->getId() << " ";
    if (e->face() != nullptr)
      std::cout << e->face()->getId() << " ";
    else
      std::cout << "0 ";
    std::cout << e->next()->getId() << " ";
    std::cout << e->prev()->getId() << "\n";
  }
}