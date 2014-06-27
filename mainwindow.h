#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "screen.h"
#include "router.h"
#include "client.h"
#include "calibrer.h"


class Router;
class Screen;
class calibrer;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateProcessList(std::map<int,Client*> map);
    void setOnScreen(QString proc_name);
    void drawLines(std::vector<Point> points);


private slots:
    void on_pb_dac_clicked();
    void on_pb_launch_clicked();
    void on_pb_stop_clicked();

    void on_pb_cal_clicked();

private:
    Ui::MainWindow *ui;
    Screen* screen;
    Router* router;
    calibrer *cal;
};

#endif // MAINWINDOW_H
