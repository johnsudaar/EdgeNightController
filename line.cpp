#include "line.h"

Line::Line()
{
}

Line::Line(Point start, Point end){
    this->start = start;
    this->end = end;
}

std::vector<Point> Line::getPoints(){
    double diff_x = (this->end.x - this->start.x);
    double diff_y = (this->end.y - this->start.y);
    int delta = sqrt(pow(diff_x,2)+pow(diff_y,2))/Parameters::get()->getLineRes();
    delta = delta > 2 ? delta : 2;
    this->end.r = this->start.r;
    this->end.g = this->start.g;
    this->end.b = this->start.b;
    double delta_x = diff_x / delta;
    double delta_y = diff_y / delta;
    std::vector<Point> points = std::vector<Point>();
    points.push_back(this->start);

    for(int i = 0; i<delta ; i++){
        Point p = Point(this->start.x + i*delta_x, this->start.y + i*delta_y, this->start.r, this->start.g, this->start.b);
        points.push_back(p);
    }

    points.push_back(this->end);
    return points;
}
