#include "line.h"

Line::Line()
{
}

Line::Line(Point start, Point end){
    this->start = start;
    this->end = end;
}

std::vector<Point> Line::getPoints(){
    std::vector<Point> points = std::vector<Point>();
    points.push_back(this->start);
    points.push_back(this->end);
    return points;
}
