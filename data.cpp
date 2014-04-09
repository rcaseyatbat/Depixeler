
#include "data.h"

int
Triangle::containEdge(const Edge &aEdge) const
{
    if (aEdge.start == v1 && aEdge.end == v2) {
        return 0; // first aEdge
    }
    if (aEdge.start == v2 && aEdge.end == v1) {
        return 0;
    }
    if (aEdge.start == v2 && aEdge.end == v3) {
        return 1; // second aEdge
    }
    if (aEdge.start == v3 && aEdge.end == v2) {
        return 1;
    }
    if (aEdge.start == v1 && aEdge.end == v3) {
        return 2; // third aEdge
    }
    if (aEdge.start == v3 && aEdge.end == v1) {
        return 2;
    }
    return -1;

}

float
Triangle::sign(const Vertex &p1, const Vertex &p2, const Vertex &p3) const
{
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool
Triangle::isPointInside(const Vertex &aPt) const
{
#if 1
  bool b1, b2, b3;

  b1 = sign(aPt, mVert1, mVert2) < 0.0f;
  b2 = sign(aPt, mVert2, mVert3) < 0.0f;
  b3 = sign(aPt, mVert3, mVert1) < 0.0f;

  return ((b1 == b2) && (b2 == b3));
#else
  const Vertex &p1 = mVert1;
  const Vertex &p2 = mVert2;
  const Vertex &p3 = mVert3;
  const Vertex &p = aPt;

  double alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
  double beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
  double gamma = 1.0f - alpha - beta;

  if ((alpha > 0) && (beta > 0) && (gamma > 0)) {
    return true;
  } else {
    return false;
  }   
#endif

}

