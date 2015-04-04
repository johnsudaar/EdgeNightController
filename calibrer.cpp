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
    connect(this->ui->hs_scan_speed,SIGNAL(valueChanged(int)), this, SLOT(other_change(int)));
    connect(this->ui->hs_line_res,SIGNAL(valueChanged(int)),this,SLOT(other_change(int)));
    connect(this->ui->hs_circle_res,SIGNAL(valueChanged(int)),this,SLOT(other_change(int)));
    this->screen = screen;
    this->size_change(0);
    this->update();
}

calibrer::~calibrer()
{
    delete ui;
}

void calibrer::size_change(int val){

    this->ui->hs_x_offset->setMaximum(65535 - this->ui->hs_width->value());
    this->ui->hs_y_offset->setMaximum(65535 - this->ui->hs_y_offset->value());
    this->update();
}

void calibrer::offset_change(int val){
    this->ui->hs_height->setMaximum(65535 - this->ui->hs_y_offset->value());
    this->ui->hs_width->setMaximum(65535 - this->ui->hs_x_offset->value());
    this->update();
}

void calibrer::other_change(int val){
    this->update();
}

void calibrer::update(){
    ui->lbl_height_value->setText(QString::number(ui->hs_height->value()));
    ui->lbl_width_value->setText(QString::number(ui->hs_width->value()));
    ui->lbl_X_Offset_Value->setText(QString::number(ui->hs_x_offset->value()));
    ui->lbl_Y_Offset_Value->setText(QString::number(ui->hs_y_offset->value()));
    ui->lbl_scan_speed_value->setText(QString::number(ui->hs_scan_speed->value()));
    ui->lbl_circle_res_value->setText(QString::number(ui->hs_circle_res->value()));
    ui->lbl_line_res_value->setText(QString::number(ui->hs_line_res->value()));
    Parameters::get()->set(this->ui->hs_width->value(), this->ui->hs_height->value(),this->ui->hs_x_offset->value(),this->ui->hs_y_offset->value(), this->ui->hs_scan_speed->value(),this->ui->hs_line_res->value(),this->ui->hs_circle_res->value());
}

void calibrer::on_pb_close_clicked()
{
    this->hide();
}
