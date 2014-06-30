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
    if(datagram.at(0) == 0){
        if(datagram.at(1)==INSTR_IN_HELLO){
            QString name = "";
            for(int i = 0; i<22; i++){
                if(datagram.at(i+2)==0)
                    break;
                else
                    name += datagram.at(i+2);
            }
            if(DEBUG)
                std::cout << "[ROUTER] Hello Received from "<<name.toStdString()<<" (Port : " << port<<")"<<std::endl;
            for(std::map<int,Client*>::iterator it = this->process.begin(); it!= this->process.end(); ++it){
                QString cur_name = it->second->getName();
                if(cur_name.compare(name) == 0){
                    if(it->second->isOnScreen())
                        this->stopProcess();
                    process.erase(it);
                }
            }
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
        unsigned short id = datagram[0]&0xFF;
        if(id == 255){
            if(DEBUG)
                std::cout<<"[ROUTER] Input controller received"<<std::endl;
            this->processInput(datagram);
        }else{
            if(process.find(id) == process.end()){ // Si l'id ne fait pas partis des ids enregistrés
                std::cerr<<"[ROUTER] Not a real id : "<<id<<std::endl;
            }else{
                if(process.at(id)->getAddress() == sender && process.at(id)->getPort() == port){
                    if(! process.at(id)->isActiv()){ // Si le programme a commis une faute et est banni
                        std::cerr<<"[ROUTER] Message received from "<<id<<" but this program was banned..."<<std::endl;
                    }else{
                        if(! process.at(id)->isOnScreen()){
                            std::cerr<<"[ROUTER] Message received from "<<id<<" but this program is not on screen..."<<std::endl;
                        }else{
                            if(datagram[1] == (char)INSTR_IN_STOP){
                                if(DEBUG)
                                    std::cout<<"[ROUTER] Instruction stop received from "<<id<<" ... Stopping process !"<<std::endl;
                                this->randomGame();
                            }else{
                                if(DEBUG)
                                    std::cout<<"[ROUTER] Instruction "<<datagram[1]<<" received"<<std::endl;
                                this->screen->fromNetwork(datagram);
                            }
                        } // Processus a l'ecran
                    }// Processus actif
                }else{
                    std::cerr<<"[ROUTER] Wrong sender"<<std::endl;
                }// Meme ip et addresse
            }// Processus existant
        }// Input controller
    } // Instruction != HELLO
}

void Router::startProcess(int pid){
    if(this->is_running){
        std::cerr<<"[ROUTER] Cannot launch : another process is running"<<std::endl;
        return;
    }

    if(process.find(pid) == process.end()){ // Si l'id ne fait pas partis des ids enregistrés
        std::cerr<<"[ROUTER] Cannot launch : not a real id : "<<pid<<std::endl;
    }else{
        if(! process.at(pid)->isActiv()){ // Si le programme a commis une faute et est banni
            std::cerr<<"[ROUTER] Cannot launch "<<pid<<" program is banned..."<<std::endl;

        }else{
            QByteArray dg = QByteArray(1,0);
            dg[0] = INSTR_OUT_GO;
            std::cout<<"[ROUTER] Launching process "<<pid<<std::endl;
            process[pid]->setOnScreen(true);
            this->is_running = true;
            this->ui->setOnScreen(process[pid]->getName());
            socket->writeDatagram(dg,process[pid]->getAddress(),process[pid]->getPort());
            this->ui->timer->start(1000);

        }// is banned
    }// exists
}

void Router::stopProcess(){
    if(! this->is_running){
        std::cerr<<"[ROUTER] Cannot stop process : No process running..."<<std::endl;
    }else{
        QByteArray dg = QByteArray(1,0);
        dg[0] = INSTR_OUT_STOP;
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
        this->ui->timer->stop();
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

void Router::processInput(QByteArray datagram){
    this->ui->inputConnected();
    if(datagram[1] == INSTR_IN_INPUT){
        if(datagram[2] == 255 || datagram[3] == 255){
            if(DEBUG)
                std::cout<<"[ROUTER] Stop received by user"<<std::endl;
            this->randomGame();
        }else{
            if(this->is_running){
                QByteArray dg = QByteArray(3,0);
                dg[0] = INSTR_OUT_INPUT;
                dg[1] = datagram[2];
                dg[2] = datagram[3];
                int pid = -1;
                for(std::map<int,Client*>::iterator it = this->process.begin(); it!= this->process.end(); ++it){
                    if(it->second->isOnScreen())
                        pid = it->first;
                }
                if(DEBUG)
                    std::cout<<"[ROUTER] Input received"<<std::endl;
                if(pid == -1){
                    std::cerr<<"[ROUTER] General failure ..."<<std::endl;
                    exit(EXIT_FAILURE);
                }
                socket->writeDatagram(dg,process[pid]->getAddress(),process[pid]->getPort());
            }else{
                std::cout<<"[ROUTER] Input received but no process runnings"<<std::endl;
            }
        }

    }else{
        std::cerr<<"[ROUTER] Input Controller : Unexpected instruction"<<std::endl;
    }
}

void Router::randomGame(){
    int game = 0;
    if(this->process.size() == 0){
        std::cerr<<"[ROUTER] No process ..."<<std::endl;
        return;
    } else if(this->process.size() == 1){
        game = this->process.begin()->first;
    }else{
        game = rand()%this->process.size()+1;
        while(this->process.find(game)== this->process.end() || ! this->process[game]->isActiv() || this->process[game]->isOnScreen())
            game = rand()%this->process.size()+1;
    }
    this->stopProcess();
    this->startProcess(game);
}

void Router::setDebug(bool d){
    this->debug_mode = d;
}

void Router::banProcess(){
    if(this->is_running){
        int pid = -1;
        for(std::map<int,Client*>::iterator it = this->process.begin(); it!= this->process.end(); ++it){
            if(it->second->isOnScreen())
                pid = it->first;
        }
        if(pid == -1){
            std::cerr<<"[ROUTER] General failure ..."<<std::endl;
            exit(EXIT_FAILURE);
        }
        std::cerr<<"[ROUTER] Ban process... "<<std::endl;
        if(! this->debug_mode){
            this->stopProcess();
            this->process[pid]->setActiv(false);
        }
    }else{
        std::cerr<<"[ROUTER] No process running ..."<<std::endl;
    }
}
