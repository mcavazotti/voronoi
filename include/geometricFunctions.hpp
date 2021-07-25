#ifndef GEOMETRIC_FUNC_H
#define GEOMETRIC_FUNC_H

#include "delaunay.hpp"

namespace geo
{

  /**
 * Create an edge, from a point to ("2") another point.
 * 
 * Used at the initialization of the bounding triange
 */
  template <typename T>
  HalfEdge<T> *createEdgeP2P(Point<T> *from, Point<T> *to, Face<T> *leftFace, Face<T> *rightFace);

  /**
 *  Create an edge, from a point to ("2") a half-edge.
 * 
 * It doesn't set the faces.
 */
  template <typename T>
  HalfEdge<T> *createEdgeP2E(Point<T> *from, HalfEdge<T> *toEdge);

  /**
 * Set the face reference in each node of the edge chain. 
*/
  template <typename T>
  void setFace(HalfEdge<T> *edgeChain, Face<T> *face);

  template <typename T>
  void insertPointInEdge(Point<T> *p, HalfEdge<T> *edge);

  /**
 * Insert diagonal and set faces.
 * 
 * Return the face created.
 */
  template <typename T>
  Face<T> *insertDiagonal(HalfEdge<T> *fromEdge, HalfEdge<T> *toEdge);

  template <typename T>
  void legalizeEdge(Point<T> *p, HalfEdge<T> *edge);

  /**
 * Remove vertex and rearrange DCEL around it.
 */
  template <typename T>
  void removeVertex(Point<T> *p);

  template <typename T>
  Point<T> computeCircuncenter(Point<T> const &p1, Point<T> const &p2, Point<T> const &p3);

  template <typename T>
  double computeDistance(Point<T> const &a, Point<T> const &b);
}

template <typename T>
Point<T> computeNormal(HalfEdge<T> const &e);

#include "../template/geometricFunctions.tpp"

#endif