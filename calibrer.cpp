#include "calibrer.h"
#include "ui_calibrer.h"

calibrer::calibrer(QWidget *parent, Screen *screen) :
    QDialog(parent),
    ui(new Ui::calibrer)
{
    ui->setupUi(this);
    connect(this->ui->hs_p1,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));
    connect(this->ui->hs_p2,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));
    connect(this->ui->hs_p3,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));
    connect(this->ui->hs_p4,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));

    connect(this->ui->vs_p1,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));
    connect(this->ui->vs_p2,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));
    connect(this->ui->vs_p3,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));
    connect(this->ui->vs_p4,SIGNAL(valueChanged(int)),this,SLOT(slide_change(int)));

    this->screen = screen;
}

calibrer::~calibrer()
{
    delete ui;
}

void calibrer::slide_change(int val){
    // P1 : (0, 0)
    // P2 : (65535, 0);
    // P3 : (65535, 65535)
    // P4 : (0, 65535)
    Point p1 = Point(this->ui->hs_p1->value(), 65535 - this->ui->vs_p1->value());
    Point p2 = Point(this->ui->hs_p2->value(), 65535 - this->ui->vs_p2->value());
    Point p3 = Point(this->ui->hs_p3->value(), 65535 - this->ui->vs_p3->value());
    Point p4 = Point(this->ui->hs_p4->value(), 65535 - this->ui->vs_p4->value());

    this->screen->setPoints(p1,p2,p3,p4);
}

void calibrer::on_pb_close_clicked()
{
    this->hide();
}
