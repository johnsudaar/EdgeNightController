#ifndef LINE_H
#define LINE_H

#include <math.h>

#include "const.h"
#include "parameters.h"
#include "displayableobject.h"
#include "point.h"

class Line : public virtual DisplayableObject
{
public:
    Line();
    virtual std::vector<Point> getPoints();
    Line(Point start, Point end);
private:
    Point start;
    Point end;
};

#endif // LINE_H
