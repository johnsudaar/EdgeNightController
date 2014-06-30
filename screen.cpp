#include "screen.h"

typedef struct
 {
  unsigned short Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7, Ch8, TTL;
 } TLumax_Point;

TLumax_Point Lumax_Points[4000];

typedef int __stdcall (*tLumax_SendFrame)(int, TLumax_Point *, int, int, int, int *);
typedef int __stdcall (*tLumax_StopFrame)(int);
typedef int __stdcall (*tLumax_WaitForBuffer)(int, int, int *, int *);
typedef int __stdcall (*tLumax_GetPhysicalDevices)();
typedef int __stdcall (*tLumax_OpenDevice)(int, int);
typedef int __stdcall (*tLumax_CloseDevice)(int);

tLumax_SendFrame fLumax_SendFrame;
tLumax_StopFrame fLumax_StopFrame;
tLumax_WaitForBuffer fLumax_WaitForBuffer;
tLumax_GetPhysicalDevices fLumax_GetPhysicalDevices;
tLumax_OpenDevice fLumax_OpenDevice;
tLumax_CloseDevice fLumax_CloseDevice;

Screen::Screen(bool dac, MainWindow *ui){
    this->dac = dac;
    this->ui = ui;
    this->objects.reserve(20);
    bool dll_loaded = false;
    HINSTANCE DllHandle;
    DllHandle = LoadLibrary("lumax.dll"); // open DLL
    if (DllHandle != NULL)
      { // DLL successfully opened -> get functions
       fLumax_SendFrame = (tLumax_SendFrame)GetProcAddress(DllHandle, "Lumax_SendFrame");
       fLumax_StopFrame = (tLumax_StopFrame)GetProcAddress(DllHandle, "Lumax_StopFrame");
       fLumax_WaitForBuffer = (tLumax_WaitForBuffer)GetProcAddress(DllHandle, "Lumax_WaitForBuffer");
       fLumax_GetPhysicalDevices = (tLumax_GetPhysicalDevices)GetProcAddress(DllHandle, "Lumax_GetPhysicalDevices");
       fLumax_OpenDevice = (tLumax_OpenDevice)GetProcAddress(DllHandle, "Lumax_OpenDevice");
       fLumax_CloseDevice = (tLumax_CloseDevice)GetProcAddress(DllHandle, "Lumax_CloseDevice");
       if (   (fLumax_SendFrame != NULL)
           && (fLumax_StopFrame != NULL)
           && (fLumax_WaitForBuffer != NULL)
           && (fLumax_GetPhysicalDevices != NULL)
           && (fLumax_OpenDevice != NULL)
           && (fLumax_CloseDevice != NULL))
        { // now we have all functions
           dll_loaded = true;
       }
    }
    if(dll_loaded){
        std::cout<<"[SCREEN] Librairie chargée !"<<std::endl;
    }else{
        std::cerr<<"[SCREEN] Erreur lors du chargement de liblumax.dll"<<std::endl;
    }
}

void Screen::addObject(DisplayableObject* obj){
    this->objects.push_back(obj);
}

void Screen::refresh(){
    std::vector<Point> points = this->getPoints();
    this->ui->drawLines(points);
    if(this->dac)
        this->toDAC(points);
    this->objects.clear();
}

bool Screen::connectDAC(){
    int nb_dac = fLumax_GetPhysicalDevices();
    if(nb_dac == 0){
        std::cerr<<"[SCREEN] No DAC connected ..."<<std::endl;
        return false;
    }
    std::cout<<"[SCREEN] "<<nb_dac<<" DAC connected !"<<std::endl;
    this->dacHandle = fLumax_OpenDevice(1, 0);
    if(this->dacHandle == 0){
        std::cerr<<"[SCREEN] Cannot open DAC ..."<<std::endl;
        return false;
    }
    //fLumax_SetTTL(this->dacHandle, 0x55);
    std::cout<<"[SCREEN] DAC connected : handle = "<<this->dacHandle<<std::endl;

    this->dac = true;
    return true;
    return false;
}

void Screen::toDAC(std::vector<Point> points){
    if(this->dac){
        TLumax_Point l_points[points.size()];
        int i = 0;
        for(std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it){
            l_points[i].Ch1 = (65535 - (*it).x)/3;
            l_points[i].Ch2 = (65535 - (*it).y)/3;
            l_points[i].Ch3 = (*it).r ? 65535 : 0;
            l_points[i].Ch4 = (*it).g ? 65535 : 0;
            l_points[i].Ch5 = (*it).b ? 65535 : 0;
            i++;
        }
        fLumax_SendFrame(this->dacHandle,l_points,points.size(),1000,10000,NULL);

    }else{
        std::cerr<<"[SCREEN] DAC not connected..."<<std::endl;
    }

}

Screen::~Screen(){
    if(this->dac){
        std::cout<<"[SCREEN] Closing DAC !";
        fLumax_CloseDevice(this->dacHandle);
    }
}


std::vector<Point> Screen::getPoints(){
    std::vector<Point> points = std::vector<Point>();
    std::vector<DisplayableObject*>::iterator it;
    for(it = this->objects.begin() ; it != this->objects.end(); ++it){
        std::vector<Point> cur = (*it)->getPoints();
        Point start = cur[0];
        start.r = false;
        start.g = false;
        start.b = false;

        Point end = cur[cur.size()-1];
        end.r = false;
        end.g = false;
        end.b = false;
        points.push_back(this->getNormalizedPoint(start));
        for(int i = 0; i<cur.size(); i++){
            points.push_back(/*this->placePoint(*/this->getNormalizedPoint(cur[i])/*)*/);
        }
        points.push_back(this->getNormalizedPoint(end));
    }

    int length = 0;

    for(int i = 1; i<points.size(); i++){
        length += sqrt(pow(points[i].x - points[i-1].x,2)+pow(points[i].y - points[i-1].y,2));
    }
    this->ui->setPoints(points.size());
    this->ui->setLength(length);
    return points;
}

Point Screen::getNormalizedPoint(Point p){
    p.x = p.x > 0 ? p.x<65535? p.x : 65535 : 0;
    p.y = p.y > 0 ? p.y<65535? p.y : 65535 : 0;
    return p;
}

unsigned short Screen::getUShort(QByteArray datagram, int offset){
    QByteArray conv = QByteArray();
    conv[0] = datagram[offset + 1];
    conv[1] = datagram[offset];
    return conv.toHex().toInt(0,16);
}

Point Screen::addColor(Point base, QByteArray datagram, int offset){
    unsigned short color = datagram[offset];
    bool r = color%2 == 1;
    color = color >> 1;
    bool g = color%2 == 1;
    color = color >> 1;
    bool b = color%2 == 1;
    base.setColor(r,g,b);
    return base;
}

void Screen::fromNetwork(QByteArray datagram){
    if(datagram[1] == (char)INSTR_IN_REFRESH){
        if(DEBUG)
            std::cout<<"[SCREEN] Refresh received ..."<<std::endl;
        this->refresh();
    }else{
        unsigned short start_x;
        unsigned short start_y;

        unsigned short end_x;
        unsigned short end_y;

        unsigned short diam;

        Point start;
        Point end;
        switch(datagram[1]){
        case INSTR_IN_LINE:
            start_x = this->getUShort(datagram,2);
            start_y = this->getUShort(datagram,4);
            start = Point(start_x, start_y);
            end_x   = this->getUShort(datagram,6);
            end_y   = this->getUShort(datagram,8);
            end  = Point(end_x, end_y);
            end = this->addColor(end,datagram,10);
            start = this->addColor(start,datagram,10);
            this->addObject(new Line(start,end));
            break;
        case INSTR_IN_RECT:
            start_x = this->getUShort(datagram,2);
            start_y = this->getUShort(datagram,4);
            start = Point(start_x, start_y);
            end_x   = this->getUShort(datagram,6);
            end_y   = this->getUShort(datagram,8);
            end  = Point(end_x, end_y);
            end = this->addColor(end,datagram,10);
            start = this->addColor(start,datagram,10);
            this->addObject(new Lol::Rectangle(start,end));
            break;
        case INSTR_IN_CIRCLE:
            start_x = this->getUShort(datagram,2);
            start_y = this->getUShort(datagram,4);
            diam    = this->getUShort(datagram,6);
            start = Point(start_x,start_y);
            start = this->addColor(start, datagram, 8);
            this->addObject(new Circle(start,diam));
            break;
        default:
            std::cout<<"[Screen] Unknown Operation"<<std::endl;
        }
    }
}
void Screen::setPoints(Point p1, Point p2, Point p3, Point p4){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;
}

/*void Screen::clearFrame(){

}*/
Point Screen::placePoint(Point p){
    double d1 = 0;
    double d2 = 0;
    double d3 = 0;
    double d4 = 0;
    // P1 : (0, 0)
    // P2 : (65535, 0);
    // P3 : (65535, 65535)
    // P4 : (0, 65535)
    d1 = sqrt(pow(p.x,2)+pow(p.y,2));
    d2 = sqrt(pow(65535 - p.x,2)+ pow(p.y,2));
    d3 = sqrt(pow(65535 - p.x,2)+ pow(65535-p.y,2));
    d4 = sqrt(pow(p.x,2)+ pow(65535-p.y,2));
    if(d1 == 0)
        return this->p1;
    if(d2 == 0)
        return this->p2;
    if(d3 == 0)
        return this->p3;
    if(d4 == 0)
        return this->p4;

    d1 = 1/d1;
    d2 = 1/d2;
    d3 = 1/d3;
    d4 = 1/d4;

    double sum = d1 + d2 + d3 + d4;

    d1 = d1/sum;
    d2 = d2/sum;
    d3 = d3/sum;
    d4 = d4/sum;

    //unsigned short r_x , r_y;
    unsigned short r_x = (double)p1.x*d1 + (double)p2.x*d2 + (double)p3.x*d3 + (double)p4.x*d4;
    unsigned short r_y = (double)p1.y*d1 + (double)p2.y*d2 + (double)p3.y*d3 + (double)p4.y*d4;
    r_x = r_x < 65535 ? r_x : 65535;
    r_y = r_y < 65535 ? r_y : 65535;

    Point r_p = Point(r_x,r_y,p.r,p.g,p.b);

    return r_p;
}
