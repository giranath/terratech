#ifndef DEF_POINT_H
#define DEF_POINT_H

namespace mapgen
{
template <class T>
struct point
{
    T x;
    T y;
    point() :x{}, y{} {}
    point(T x, T y) :x{ x }, y{ y } {}

    T operator+(const point<T>& p)
    {
        return point(x + p.x, y + p.y);
    }

    bool operator==(const point<T>& p)
    {
        return x == p.x && y == p.y;
    }
};
}
#endif

