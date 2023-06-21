#ifndef CONVEX_DECOMP_H
#define CONVEX_DECOMP_H

#include <vector>
#include <algorithm>
#include "../../thirdparty/glm/glm.hpp"

typedef float Scalar;
typedef glm::vec2 Point;
typedef std::vector<Point> Polygon;

class ConvexDecomp
{

public:
    std::vector<Polygon> polys;
    std::vector<Point> steinerPoints, reflexVertices;

    void makeCCW(Polygon &poly);
    void decomposePoly(Polygon poly);    

private:
    template <class T>
    T &at(std::vector<T> v, int i);

    bool isReflex(const Polygon &poly, const int &i);
    Point intersection(const Point &p1, const Point &p2, const Point &q1, const Point &q2);
    void swap(int &a, int &b);
    Scalar area(const Point &a, const Point &b, const Point &c);
    bool left(const Point &a, const Point &b, const Point &c);
    bool right(const Point &a, const Point &b, const Point &c);
    bool eq(const Scalar &a, const Scalar &b);
    bool rightOn(const Point &a, const Point &b, const Point &c);
    bool leftOn(const Point &a, const Point &b, const Point &c);
    Scalar sqdist(const Point &a, const Point &b);
    int wrap(const int &a, const int &b);
};

#endif