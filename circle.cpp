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
    unsigned short r = this->size/2;
    int reso = this->size/Parameters::get()->getCircleRes() > 6 ? this->size/Parameters::get()->getCircleRes() : 6;
    double inc = 2*PI/reso;

    for(int i = 0; i<reso; i++){
        int x = cos(inc*i)*r + this->center.x;
        int y = sin(inc*i)*r + this->center.y;
        x = x < 0 ? 0 : x > MAX_X ? MAX_X : x; // Normalize x
        y = y < 0 ? 0 : y > MAX_Y ? MAX_Y : y; // Normalize y
        points.push_back(Point(x,y,this->center.r, this->center.g, this->center.b));
    }
    points.push_back(points[0]); // Close the circle
    points.push_back(points[1]); // Re-closing
    return points;
}
