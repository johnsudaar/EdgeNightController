#include "circle.h"

Circle::Circle()
{
}

Circle::Circle(Point center, unsigned short size){
    this->center = center;
    this->size  = size;
}

std::vector<Point> Circle::getPoints(){
    std::vector<Point> points;
    double inc = 2*PI/CIRCLE_RES;
    unsigned short r = this->size/2;
    for(int i = 0; i<CIRCLE_RES; i++){
        int x = cos(inc*i)*r + this->center.x;
        int y = sin(inc*i)*r + this->center.y;
        x = x < 0 ? 0 : x > MAX_X ? MAX_X : x; // Normalize x
        y = y < 0 ? 0 : y > MAX_Y ? MAX_Y : y; // Normalize y
        points.push_back(Point(x,y,this->center.r, this->center.g, this->center.b));
    }
    points.push_back(points[0]); // Close the circle
    return points;
}
