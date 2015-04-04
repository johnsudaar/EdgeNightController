#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(server_timer()));

    ui->setupUi(this);
    this->screen = new Screen(false, this);
    this->router = new Router(this->screen, this);
    this->ui->gv_Visualizer->setBackgroundBrush(Qt::black);
    this->ui->gv_Visualizer->setScene(new QGraphicsScene());
    Parameters::get()->setScreen(this->screen);
    this->drawLines(std::vector<Point>());
    this->cal = new calibrer(0,this->screen);

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
    screen->clearFrame();
}

void MainWindow::drawLines(std::vector<Point> points){

    QGraphicsScene * scene = this->ui->gv_Visualizer->scene();
    scene->clear();
    QRect geo = this->ui->gv_Visualizer->rect();
    scene->setBackgroundBrush(Qt::black);
    QPen pen;
    Point previous;
    for(std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it){
        if(it == points.begin()){
            previous = *it;
        }else{
            int alpha = 255;
            if((previous.r || previous.g || previous.b)){
                pen = QPen(QColor(previous.r?255:0,previous.g?255:0,previous.b?255:0,alpha));
            }else{
                pen = QPen(QColor(0,255,0,70));
                pen.setStyle(Qt::DashDotDotLine);
            }
            scene->addLine(previous.x*geo.width()/65535,previous.y*geo.height()/65535,(*it).x*geo.width()/65535,(*it).y*geo.height()/65535,pen);
            previous = *it;
        }
    }
    this->ui->gv_Visualizer->setScene(scene);
    this->ui->gv_Visualizer->update();

}

void MainWindow::on_pb_dac_clicked()
{
    this->ui->pb_dac->setEnabled(! this->screen->connectDAC());
}

void MainWindow::on_pb_cal_clicked()
{
    this->cal->show();
    this->cal->setFocus();
}

void MainWindow::setPoints(int nb_points){
    QString text = "Points : " + QString::number(nb_points);
    this->ui->lbl_points->setText(text);
}

void MainWindow::setLength(int length){
    QString text = "Length : " + QString::number(length);
    this->ui->lbl_length->setText(text);
}

void MainWindow::on_pb_rand_clicked()
{
    this->router->randomGame();
}

void MainWindow::server_timer(){
    QString fr = "FPS : "+QString::number(this->frames);
    if(this->frames < MIN_FPS)
        this->router->banProcess();
    this->frames = 0;
    QString ob = "OPS : "+QString::number(this->objs);
    this->objs = 0;
    this->ui->lbl_ops->setText(ob);
    this->ui->lbl_fps->setText(fr);
}

void MainWindow::addFrame(int objects){
    this->frames++;
    this->objs += objects;
}

void MainWindow::on_checkBox_clicked()
{
    this->router->setDebug(this->ui->cb_debug->isChecked());
}

void MainWindow::inputConnected(){
    this->ui->lbl_input->setText("Input Controller : connected");
}

void MainWindow::on_pb_test_pattern_clicked()
{
    this->router->stopProcess();
    this->screen->sendTestPattern();
}
