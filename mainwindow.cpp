#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->screen = new Screen(false, this);
    this->router = new Router(this->screen, this);
    this->ui->gv_Visualizer->setBackgroundBrush(Qt::black);

    this->drawLines(std::vector<Point>());
    this->cal = new calibrer(0,this->screen);

    this->screen->setPoints(Point(0,0), Point(65535,0), Point(65535,65535), Point(0,65535));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->screen;
}

void MainWindow::updateProcessList(std::map<int,Client*> map){
    this->ui->cb_process->clear();
    for(std::map<int,Client*>::iterator it = map.begin(); it != map.end(); ++it){
        this->ui->cb_process->addItem(it->second->getName());
    }
}

void MainWindow::setOnScreen(QString proc_name){
    QString disp = QString("Running : ");
    disp +=proc_name;
    this->ui->lbl_running->setText(disp);
}

void MainWindow::on_pb_launch_clicked()
{
    int pid = router->getProcessFromName(this->ui->cb_process->currentText());
    router->startProcess(pid);
}

void MainWindow::on_pb_stop_clicked()
{
    router->stopProcess();
}

void MainWindow::drawLines(std::vector<Point> points){
    QGraphicsScene * scene = new QGraphicsScene(this);
    QRect geo = QRect(0,0,65535,65535);
    scene->setSceneRect(geo);
    scene->setBackgroundBrush(Qt::black);
    Point previous;
    for(std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it){
        if(it == points.begin()){
            previous = *it;
        }else{
            int alpha = 255;
            if(!(previous.r || previous.g || previous.b)){
                alpha = 0;
            }
            QPen pen = QPen(QColor(previous.r?255:0,previous.g?255:0,previous.b?255:0,alpha));
            scene->addLine(previous.x,previous.y,(*it).x,(*it).y,pen);
            previous = *it;
        }
    }
    this->ui->gv_Visualizer->setScene(scene);
    this->ui->gv_Visualizer->fitInView(geo, Qt::KeepAspectRatio );

}

void MainWindow::on_pb_dac_clicked()
{
    this->ui->pb_dac->setEnabled(! this->screen->connectDAC());
}

void MainWindow::on_pb_cal_clicked()
{
    this->cal->show();
}