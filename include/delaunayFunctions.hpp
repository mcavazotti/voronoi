#ifndef DELAUNAY_FUNC_H
#define DELAUNAY_FUNC_H

#include "delaunay.hpp"

/**
 * Create an edge, from a point to ("2") another point.
 * 
 * Used at the initialization of the bounding triange
 */
HalfEdge *createEdgeP2P(PointInt *from, PointInt *to, Face *leftFace, Face *rightFace);

/**
 *  Create an edge, from a point to ("2") a half-edge.
 * 
 * It doesn't set the faces.
 */
HalfEdge *createEdgeP2E(PointInt *from, HalfEdge *toEdge);

/**
 * Set the face reference in each node of the edge chain. 
*/
void setFace(HalfEdge *edgeChain, Face *face);

void insertPointInEdge(PointInt *p, HalfEdge *edge);

/**
 * Insert diagonal and set faces.
 * 
 * Return the face created.
 */
Face * insertDiagonal(HalfEdge *fromEdge, HalfEdge *toEdge);


void legalizeEdge(PointInt *p, HalfEdge *edge);

#endif