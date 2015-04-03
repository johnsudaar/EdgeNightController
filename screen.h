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
//#include <windows.h>

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
    void clearFrame();
    void sendTestPattern();
    bool connectDAC();
    void setParameters(int width, int height, int offset_x, int offset_y, int scan_speed);
    Point placePoint(Point p);

private:
    unsigned short getUShort(QByteArray datagram, int offset);
    bool dac;
    bool test_pattern;
    int dacHandle;
    std::vector<DisplayableObject*> objects;
    std::vector<Point> getPoints();
    Point getNormalizedPoint(Point p);
    Point addColor(Point base, QByteArray datagram, int offset);
    MainWindow *ui;
    int width;
    int height;
    int offset_x;
    int offset_y;
    int scan_speed;
};

#endif // SCREEN_H
