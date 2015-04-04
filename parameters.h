#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "screen.h"

class Screen;

class Parameters
{
public:
    static Parameters* get();
    void set(int width, int height, int offset_x, int offset_y, int scan_speed, int line_res, int circle_res);
    void setScreen(Screen* scr);
    int getWidth();
    int getHeight();
    int getOffsetX();
    int getOffsetY();
    int getScanSpeed();
    int getLineRes();
    int getCircleRes();
    Parameters();
    ~Parameters();
private:
    int line_res;
    int circle_res;
    int width;
    int height;
    int offset_x;
    int offset_y;
    int scan_speed;
    Screen* screen;
    static Parameters* instance;
};

#endif // PARAMETERS_H
