#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "displayableobject.h"

namespace Lol{

class Rectangle : public virtual DisplayableObject
{
public:
    Rectangle();
    Rectangle(Point p1, Point p2);
    virtual std::vector<Point> getPoints();
private:
    Point p1;
    Point p2;
};
}

#endif // RECTANGLE_H
