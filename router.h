#ifndef ROUTER_H
#define ROUTER_H

#include "const.h"
#include "screen.h"
#include "client.h"
#include "mainwindow.h"
#include <QUdpSocket>
#include <QObject>
#include <QString>
#include <iostream>
#include <cstdlib>

class MainWindow;
class Screen;

class Router : public QObject
{
    Q_OBJECT
public:
    Router(Screen* screen, MainWindow *ui);
    void startProcess(int pid);
    int getProcessFromName(QString name);
    void stopProcess();
    void randomGame();
    void banProcess();
    void setDebug(bool d);
private:
    Screen* screen;
    std::map<int, Client*> process;
    QUdpSocket* socket;
    void processDatagram(QByteArray datagram, QHostAddress sender, quint16 port);
    void processInput(QByteArray datagram);
    bool is_running;
    MainWindow *ui;
    unsigned short cur_id;
    bool debug_mode = false;

private slots:
    void readDatagram();


};

#endif // ROUTER_H
