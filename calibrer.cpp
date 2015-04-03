#include "calibrer.h"
#include "ui_calibrer.h"

calibrer::calibrer(QWidget *parent, Screen *screen) :
    QDialog(parent),
    ui(new Ui::calibrer)
{
    ui->setupUi(this);
    connect(this->ui->hs_height,SIGNAL(valueChanged(int)),this,SLOT(size_change(int)));
    connect(this->ui->hs_width,SIGNAL(valueChanged(int)),this,SLOT(size_change(int)));
    connect(this->ui->hs_x_offset,SIGNAL(valueChanged(int)),this,SLOT(offset_change(int)));
    connect(this->ui->hs_y_offset,SIGNAL(valueChanged(int)),this,SLOT(offset_change(int)));
    connect(this->ui->hs_scan_speed,SIGNAL(valueChanged(int)), this, SLOT(offset_change(int)));
    this->screen = screen;
}

calibrer::~calibrer()
{
    delete ui;
}

void calibrer::size_change(int val){

    this->ui->hs_x_offset->setMaximum(65535 - this->ui->hs_width->value());
    this->ui->hs_y_offset->setMaximum(65535 - this->ui->hs_y_offset->value());
    this->screen->setParameters(this->ui->hs_width->value(), this->ui->hs_height->value(),this->ui->hs_x_offset->value(),this->ui->hs_y_offset->value(), this->ui->hs_scan_speed->value());
}

void calibrer::offset_change(int val){
    this->ui->hs_height->setMaximum(65535 - this->ui->hs_y_offset->value());
    this->ui->hs_width->setMaximum(65535 - this->ui->hs_x_offset->value());
    this->screen->setParameters(this->ui->hs_width->value(), this->ui->hs_height->value(),this->ui->hs_x_offset->value(),this->ui->hs_y_offset->value(), this->ui->hs_scan_speed->value());
}

void calibrer::on_pb_close_clicked()
{
    this->hide();
}
