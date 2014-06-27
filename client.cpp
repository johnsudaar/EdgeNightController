#include "client.h"

Client::Client(QString name, QHostAddress address, quint16 port){
    this->name = name;
    this->address = address;
    this->port =port;
    this->activ = true;
    this->onScreen = false;
}

void Client::setActiv(bool activ){
    this->activ = activ;
}

void Client::setOnScreen(bool onScreen){
    this->onScreen = onScreen;
}

bool Client::isActiv(){
    return this->activ;
}

bool Client::isOnScreen(){
    return this->onScreen;
}

QString Client::getName(){
    return this->name;
}

quint16 Client::getPort(){
    return this->port;
}

QHostAddress Client::getAddress(){
    return this->address;
}

int Client::getId(){
    return this->id;
}
