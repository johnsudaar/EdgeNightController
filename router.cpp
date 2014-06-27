#include "router.h"

Router::Router(Screen* screen, MainWindow *ui)
{
    this->process = std::map<int,Client*>();
    socket = new QUdpSocket();
    socket->bind(QHostAddress::Any, LISTEN_PORT);
    std::cout<<"[ROUTER] Server listenning ..." <<std::endl;
    connect(socket, SIGNAL(readyRead()),this, SLOT(readDatagram()));
    this->is_running = false;
    this->ui = ui;
    this->screen = screen;
    this->cur_id = 0;
}

void Router::readDatagram()
 {
     while (socket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(socket->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;
         socket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);
         processDatagram(datagram, sender, senderPort);
     }
 }

void Router::processDatagram(QByteArray datagram, QHostAddress sender, quint16 port){
    if(datagram.length() != 24){
        std::cerr<<"[ROUTER] Datagram malformed (wrong size) ... " <<std::endl;
        //return;
    }
    if(datagram.at(0) == 0){
        if(datagram.at(1)==INSTR_IN_HELLO){
            QString name = "";
            for(int i = 0; i<22; i++){
                if(datagram.at(i+2)==0)
                    break;
                else
                    name += datagram.at(i+2);
            }
            std::cout << "[ROUTER] Hello Received from "<<name.toStdString()<<" (Port : " << port<<")"<<std::endl;
            QByteArray dg = QByteArray(2,0);
            dg[0] = INSTR_OUT_ACK;
            dg[1] = ++this->cur_id;
            std::cout <<"[ROUTER] Giving id : "<<(int)dg[1]<<" to "<<name.toStdString()<<std::endl;
            Client* cur_cli = new Client(name,sender,port);
            process[(int)dg[1]] = cur_cli;
            socket->writeDatagram(dg,sender,port);
            this->ui->updateProcessList(process);
        }else{
            std::cerr<<"[ROUTER] Datagram malformed (id = 0, but not HELLO)"<<std::endl;
        }
    }else{
        int id = (int)datagram[0];
        if(process.find(id) == process.end()){ // Si l'id ne fait pas partis des ids enregistrés
            std::cerr<<"[ROUTER] Not a real id : "<<id<<std::endl;
        }else{
            if(! process.at(id)->isActiv()){ // Si le programme a commis une faute et est banni
                std::cerr<<"[ROUTER] Message received from "<<id<<" but this program was banned..."<<std::endl;
            }else{
                if(! process.at(id)->isOnScreen()){
                    std::cerr<<"[ROUTER] Message received from "<<id<<" but this program is not on screen..."<<std::endl;
                }else{
                    if(datagram[1] == (char)INSTR_IN_STOP){
                        std::cout<<"[ROUTER] Instruction stop received from "<<id<<" ... Stopping process !"<<std::endl;
                        process.at(id)->setOnScreen(false);
                        this->ui->setOnScreen("None ...");
                        this->is_running = false;
                    }else{
                        std::cout<<"[ROUTER] Instruction "<<datagram[1]<<" received"<<std::endl;
                        this->screen->fromNetwork(datagram);
                    }
                } // Processus a l'ecran
            }// Processus actif
        }// Processus existant
    } // Instruction != HELLO
}

void Router::startProcess(int pid){
    if(this->is_running){
        std::cerr<<"[ROUTER] Cannot launch : another process is running"<<std::endl;
        return;
    }

    if(process.find(pid) == process.end()){ // Si l'id ne fait pas partis des ids enregistrés
        std::cerr<<"[ROUTER] Cannot laynch : not a real id : "<<pid<<std::endl;
    }else{
        if(! process.at(pid)->isActiv()){ // Si le programme a commis une faute et est banni
            std::cerr<<"[ROUTER] Cannot launch "<<pid<<" program is banned..."<<std::endl;

        }else{
            QByteArray dg = QByteArray(2,0);
            dg[0] = INSTR_OUT_GO;
            dg[1] = 0;
            std::cout<<"[ROUTER] Launching process "<<pid<<std::endl;
            process[pid]->setOnScreen(true);
            this->is_running = true;
            this->ui->setOnScreen(process[pid]->getName());
            socket->writeDatagram(dg,process[pid]->getAddress(),process[pid]->getPort());

        }// is banned
    }// exists
}

void Router::stopProcess(){
    if(! this->is_running){
        std::cerr<<"[ROUTER] Cannot stop process : No process running..."<<std::endl;
    }else{
        QByteArray dg = QByteArray(2,0);
        dg[0] = INSTR_OUT_STOP;
        dg[1] = 0;
        int pid = -1;
        for(std::map<int,Client*>::iterator it = this->process.begin(); it!= this->process.end(); ++it){
            if(it->second->isOnScreen())
                pid = it->first;
        }
        if(pid == -1){
            std::cerr<<"[ROUTER] General failure ..."<<std::endl;

            exit(EXIT_FAILURE);
        }
        std::cout<<"Stopping process : "<<pid<<std::endl;
        process[pid]->setOnScreen(false);
        this->is_running = false;
        this->ui->setOnScreen("None ...");
        socket->writeDatagram(dg,process[pid]->getAddress(),process[pid]->getPort());
    }

}

int Router::getProcessFromName(QString name){
    int pid = -1;
    for(std::map<int,Client*>::iterator it = this->process.begin(); it!= this->process.end(); ++it){
        if(name.compare(it->second->getName()) == 0)
            pid = it->first;
    }
    return pid;
}

