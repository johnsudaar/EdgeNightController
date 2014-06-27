#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QUdpSocket>


class Client
{
public:
    Client(QString name, QHostAddress address, quint16 port);
    void setActiv(bool activ);
    void setOnScreen(bool onScreen);
    bool isActiv();
    bool isOnScreen();
    QString getName();
    quint16 getPort();
    QHostAddress getAddress();
    int getId();
private:
    bool activ;
    bool onScreen;
    QString name;
    QHostAddress address;
    quint16 port;
    int id;
};

#endif // CLIENT_H
