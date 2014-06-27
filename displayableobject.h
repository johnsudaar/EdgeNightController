#ifndef DISPLAYABLEOBJECT_H
#define DISPLAYABLEOBJECT_H
#include "point.h"
#include <vector>

class DisplayableObject
{
public:
    DisplayableObject();
    virtual std::vector<Point> getPoints() = 0;
};

#endif // DISPLAYABLEOBJECT_H
