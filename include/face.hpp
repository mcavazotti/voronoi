#ifndef FACE_H

#define FACE_H
#include "halfEdge.hpp"

template <typename T>
class HalfEdge;

/**
 * This class represents of a face or partition.
 * 
 * It has an internal counter and a static array that stores every Face instance
*/
template <typename T>
class Face
{
public:
  Face() : eC(nullptr), id(-1) {}

  HalfEdge<T> *edgeChain() const { return eC; }

  void setChain(HalfEdge<T> *e)
  {
    eC = e;
  }

  void setId(int i)
  {
    id = i;
  }

  int getFaceSize()
  {
    HalfEdge<T> *tmp = eC;
    int numPoints = 0;

    do
    {
      numPoints++;
      tmp = tmp->next();
    } while (tmp != eC);
    return numPoints;
  }
  int getId() const { return id; };

public:
private:
  HalfEdge<T> *eC;
  int id;
};

#endif