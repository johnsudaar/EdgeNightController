#include "parameters.h"

Parameters* Parameters::instance = NULL;

Parameters::Parameters()
{
    this->screen = NULL;
}

Parameters::~Parameters()
{

}

Parameters* Parameters::get(){
    if(Parameters::instance == NULL){
        Parameters::instance = new Parameters();
    }
    return Parameters::instance;
}

void Parameters::setScreen(Screen* scr){
    this->screen = scr;
}

void Parameters::set(int width, int height, int offset_x, int offset_y, int scan_speed, int line_res, int circle_res){
    this->width = width;
    this->height = height;
    this->offset_x = offset_x;
    this->offset_y = offset_y;
    this->scan_speed = scan_speed;
    this->line_res = line_res;
    this->circle_res = circle_res;
    if(this->screen != NULL){
        this->screen->updateTestPattern();
    }
}

int Parameters::getWidth(){
    return this->width;
}

int Parameters::getHeight(){
    return this->height;
}

int Parameters::getOffsetX(){
    return this->offset_x;
}

int Parameters::getOffsetY(){
    return this->offset_y;
}

int Parameters::getScanSpeed(){
    return this->scan_speed;
}

int Parameters::getLineRes(){
    return this->line_res;
}

int Parameters::getCircleRes(){
    return this->circle_res;
}

