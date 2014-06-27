#include "rectangle.h"

using namespace Lol;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(Point p1, Point p2){
    this->p1 = p1;
    this->p2 = p2;
}

std::vector<Point> Rectangle::getPoints(){
    std::vector<Point> points = std::vector<Point>();
    short startx = p1.x, starty = p1.y, endx = p2.x, endy=p2.y;
    bool r = p1.r, g=p1.g, b=p1.b;
    points.push_back(Point(startx,starty,r,g,b));
    points.push_back(Point(endx, starty,r,g,b));
    points.push_back(Point(endx, endy,r,g,b));
    points.push_back(Point(startx, endy,r,g,b));
    points.push_back(Point(startx,starty,r,g,b));
    return points;
}

