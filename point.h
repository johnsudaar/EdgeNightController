#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point(){}
    Point(unsigned short x, unsigned short y, bool r, bool g, bool b){this->x = x; this->y = y; this->r = r; this->g = g; this->b = b;}

    Point(unsigned short x,unsigned short y){this->x = x; this->y=y;}

    void setColor(bool r, bool g, bool b){this->r = r; this->b = b; this->g = g;}
    unsigned short x;
    unsigned short y ;
    bool r;
    bool g;
    bool b;
};
#endif // POINT_H
