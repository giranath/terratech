#ifndef DEF_POINT_H
#define DEF_POINT_H
#include <cmath>

template <class T>
struct point
{
    T x;
    T y;

    point() : x(T{}), y({}) {}
    point(T x, T y) : x(x), y(y) {}

};
#endif // !DEF_POINT_H

