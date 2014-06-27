#ifndef CIRCLE_H
#define CIRCLE_H

#include "displayableobject.h"
#include "const.h"
#include <cmath>

class Circle : public virtual DisplayableObject
{
public:
    Circle();
    Circle(Point center, unsigned short size);
    virtual std::vector<Point> getPoints();
private:
    Point center;
    unsigned short size;
};

#endif // CIRCLE_H
