#ifndef __DATA_H
#define __DATA_H

#include <string>
#include <vector>
#include "vec3.h"

using namespace std;

struct Vector3 {
	double x;
	double y;
	double z;
};

struct Vector4 {
	double x;
	double y;
	double z;
	double w;
};

class Vertex 
{
public:
    Vertex()
        : index(0)
        , x(0)
        , y(0)
        , z(0)
        , boundary(0)
        , vorticity(0)
        , triIndex(-1)
    {};

    Vertex(double aX, double aY) 
        : index(0)
        , x(aX)
        , y(aY)
        , z(0)
        , boundary(0)
        , vorticity(0)
        , triIndex(-1)
    {};

    Vertex(int aIndex, double aX, double aY, double aBoundary) 
        : index(aIndex)
        , x(aX)
        , y(aY)
        , z(0)
        , boundary(aBoundary)
        , vorticity(0)
        , triIndex(-1)
    {};

    int index;
    double x;
    double y;
    double z;
    int boundary;
    double vorticity;
    int triIndex; // use to set triIndex for DualFace vertex
};

class Edge {
public:
    Edge()
        : index(0)
        , start(0)
        , end(0)
        , boundary(0)
        , midX(0)
        , midY(0)
        , dualEdge1(0)
        , dualEdge2(0)
        , triangle1(-1)
        , triangle2(-1)
        , flux(0)
    {};

    Edge(int aIndex, int aStart, int aEnd, int aBoundary)
        : index(aIndex)
        , start(aStart)
        , end(aEnd)
        , boundary(aBoundary)
        , midX(0)
        , midY(0)
        , dualEdge1(0)
        , dualEdge2(0)
        , triangle1(-1)
        , triangle2(-1)
        , flux(0)
    {};

    int index, start, end, boundary;
    double midX, midY;
    int dualEdge1, dualEdge2;
    int triangle1, triangle2;
    double flux;
};

class DualEdge {
public:
    DualEdge() {
	index = 0;
	start = 0;
	end = 0;
	xDirection = 0;
	yDirection = 0;
    };
    int index;
    int start;
    int end;
    //int boundary;

    double xDirection;
    double yDirection;
};

class Triangle
{
public:
    Triangle(int aIndex, int aV1, int aV2, int aV3,
             Vertex &aVert1, Vertex &aVert2, Vertex &aVert3) 
        : index(aIndex)
        , v1(aV1)
        , v2(aV2)
        , v3(aV3)
        , mVert1(aVert1)
        , mVert2(aVert2)
        , mVert3(aVert3)
        , e1(0)
        , e2(0)
        , e3(0)
        , vx(0)
        , vy(0)
    {
        x = (mVert1.x + mVert2.x + mVert3.x) / 3.0;
        y = (mVert1.y + mVert2.y + mVert3.y) / 3.0;
        backtrackX = x;
        backtrackY = y;
    };

    int containEdge(const Edge &aEdge) const;
    bool isPointInside(const Vertex &aPoint) const;
    float sign(const Vertex &p1, const Vertex &p2, const Vertex &p3) const;
    Vertex getCenter() const { return Vertex(x, y); }

    int index;
    int v1;
    int v2;
    int v3;

    Vertex mVert1;
    Vertex mVert2;
    Vertex mVert3;

    int e1;
    int e2;
    int e3;

    double x;
    double y;
    double vx;
    double vy;
    double backtrackX;
    double backtrackY;

};

#endif // __DATA_H
