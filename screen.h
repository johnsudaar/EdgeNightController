#ifndef SCREEN_H
#define SCREEN_H

#include "displayableobject.h"
#include "point.h"
#include "line.h"
#include "rectangle.h"
#include "circle.h"
#include "const.h"
#include "mainwindow.h"

#include <QUdpSocket>
#include <iostream>
#include <math.h>
#undef UNICODE
#include <windows.h>

class MainWindow;

class Screen
{
public:
    Screen(bool dac, MainWindow *ui);
    ~Screen();
    void addObject(DisplayableObject* obj);
    void refresh();
    void fromNetwork(QByteArray datagram);
    void toDAC(std::vector<Point> points);
    bool connectDAC();
    void setPoints(Point p1, Point p2, Point p3, Point p4);
    Point placePoint(Point p);

private:
    unsigned short getUShort(QByteArray datagram, int offset);
    bool dac;
    int dacHandle;
    std::vector<DisplayableObject*> objects;
    std::vector<Point> getPoints();
    Point getNormalizedPoint(Point p);
    Point addColor(Point base, QByteArray datagram, int offset);
    MainWindow *ui;
    Point p1;
    Point p2;
    Point p3;
    Point p4;
};

#endif // SCREEN_H
