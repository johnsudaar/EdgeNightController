#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "displayableobject.h"
#include "line.h"
#include "const.h"

namespace Lol{

class Rectangle : public virtual DisplayableObject
{
public:
    Rectangle();
    Rectangle(Point p1, Point p2);
    virtual std::vector<Point> getPoints();
private:
    Line l1;
    Line l2;
    Line l3;
    Line l4;
};
}

#endif // RECTANGLE_H
