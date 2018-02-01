#ifndef DEF_POINT_H
#define DEF_POINT_H
#include <cmath>

struct point
{
    double x;
    double y;

    point() : x(0), y(0) {}
    point(double x, double y) : x(x), y(y) {}

    double distance(const point& p)
    {
        return sqrt(pow((x - p.x), 2) + pow((y - p.y), 2));
    }
};
#endif // !DEF_POINT_H

