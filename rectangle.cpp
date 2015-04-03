#include "rectangle.h"

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(Point p1, Point p2){
    Point p3 = Point(p1.x, p2.y, p1.r,p1.g,p1.b);
    Point p4 = Point(p2.x, p1.y, p1.r,p1.g,p1.b);
    this->l1 = Line(p1,p3);
    this->l2 = Line(p3,p2);
    this->l3 = Line(p2,p4);
    this->l4 = Line(p4,p1);
}

std::vector<Point> Rectangle::getPoints(){
    std::vector<Point> points = std::vector<Point>();
    std::vector<Point> p_l = this->l1.getPoints();
    for(std::vector<Point>::iterator it = p_l.begin(); it != p_l.end(); ++it){
        points.push_back(*it);
    }
    p_l = this->l2.getPoints();
    for(std::vector<Point>::iterator it = p_l.begin(); it != p_l.end(); ++it){
        points.push_back(*it);
    }
    p_l = this->l3.getPoints();
    for(std::vector<Point>::iterator it = p_l.begin(); it != p_l.end(); ++it){
        points.push_back(*it);
    }
    p_l = this->l4.getPoints();
    for(std::vector<Point>::iterator it = p_l.begin(); it != p_l.end(); ++it){
        points.push_back(*it);
    }
    return points;
}

