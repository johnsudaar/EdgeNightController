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
    this->loading = false;
    this->screen = screen;
    this->size_change(0);
    if(! QFile("calibration.ini").exists()){
        this->saveSettings();
    }else{
        this->loadSettings();
    }
    this->update();
}

calibrer::~calibrer()
{
    delete ui;
}

void calibrer::size_change(int val){
        this->ui->hs_x_offset->setMaximum(65535 - this->ui->hs_width->value());
        this->ui->hs_y_offset->setMaximum(65535 - this->ui->hs_height->value());
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

void calibrer::on_pb_save_clicked()
{
    this->saveSettings();
}

void calibrer::loadSettings(){
    QSettings settings("calibration.ini",QSettings::IniFormat);
    ui->hs_height->setValue(0);
    ui->hs_width->setValue(0);
    ui->hs_x_offset->setValue(0);
    ui->hs_y_offset->setValue(0);
    ui->hs_height->setValue(settings.value("Geometry/height", 2000).toInt());
    ui->hs_width->setValue(settings.value("Geometry/width", 2000).toInt());
    ui->hs_x_offset->setValue(settings.value("Geometry/offset_x", 0).toInt());
    ui->hs_y_offset->setValue(settings.value("Geometry/offset_y", 0).toInt());
    ui->hs_scan_speed->setValue(settings.value("Laser/scan_speed",1000).toInt());
    ui->hs_line_res->setValue(settings.value("Resolutions/line",1000).toInt());
    ui->hs_circle_res->setValue(settings.value("Resolution/line",1000).toInt());
    this->update();
}

void calibrer::saveSettings(){
    QSettings settings("calibration.ini", QSettings::IniFormat);
    settings.beginGroup("Geometry");
    settings.setValue("height",ui->hs_height->value());
    settings.setValue("width",ui->hs_width->value());
    settings.setValue("offset_x",ui->hs_x_offset->value());
    settings.setValue("offset_y",ui->hs_y_offset->value());
    settings.endGroup();
    settings.beginGroup("Laser");
    settings.setValue("scan_speed",ui->hs_scan_speed->value());
    settings.endGroup();
    settings.beginGroup("Resolutions");
    settings.setValue("line",ui->hs_line_res->value());
    settings.setValue("circle",ui->hs_circle_res->value());
    settings.endGroup();
    settings.sync();
}

void calibrer::on_pb_load_clicked()
{
    this->loadSettings();
}
