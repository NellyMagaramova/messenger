/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Client.cpp
 * Author: nelly
 *
 * Created on December 4, 2016, 11:29 PM
 */

#include "Client.h"
#include <netinet/in.h>
#include <stdlib.h>
#define PREFIX "CLIENT: "
#include <string.h>
#define MAX_BUFFER 4095
#define PORT 22000
#include <stdint.h>
#include <stddef.h>
//#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <QCoreApplication>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <QtGui>
#include <memory.h>
#include <string.h>
#include <QFileDialog>
#include <QDebug>
#include <QSocketNotifier>

#define PORT 22000
using namespace std;

Client::Client() {
    connectToServer(PORT, (char*)"127.0.0.1");
    widget.setupUi(this);
//    if(widget.sendButton->click()){
    widget.ViewTextBrowser->setAcceptRichText(true);
    connect(widget.sendButton, SIGNAL(clicked()), this, SLOT(textSend()));
    connect(widget.chooseFILE, SIGNAL(clicked()), this, SLOT(sendFile()));
}


Client::~Client() {
    disconnectToServer();
}

void Client::recvData(void){


    QByteArray recvBuffer;
    recvBuffer.resize(MAX_BUFFER);

    int dataread = ::read(socket_fd, recvBuffer.data(), recvBuffer.size());
    if(dataread > 0) {
        recvBuffer.resize(dataread);

        if(recvBuffer.at(0)==1){
            recvBuffer.remove(0,1);

        widget.ViewTextBrowser->append("friend:   "+QString("<font color=\"red\">%1</font>").arg(QString::fromUtf8(recvBuffer)));
        qDebug() << "data read:" << recvBuffer;
                                 }

        else if(recvBuffer.at(0)==2){
        widget.ViewTextBrowser->append("                                                                receive file");
        recvBuffer.remove(0,1);
        QString fileName=QFileDialog::getSaveFileName(this,QString::fromUtf8("Save File"),QDir::currentPath(),"Images(*);;All files(*.*)");
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        file.write(recvBuffer);
        file.close();


        }
                        }
}

void Client::recvText(void){


}

void Client::sendFile(void){
//QString filename = QFileDialog:rgetExistingDirectory(0, "Directory Dialog", "");
QString filename =  QFileDialog::getOpenFileName(this,QString::fromUtf8("Open a file"), QDir::currentPath(),"Images(*.png *.xpm *.jpg *jpeg);;All files (*.*)");


QFile file(filename);
QByteArray filedata = file.readAll();
filedata.insert(0,'\x2');
write(socket_fd, filedata.data(), filedata.size());
widget.ViewTextBrowser->append("                                                                      file was sended");

    


}

void Client:: textSend(void){
    qDebug() << "Client:: textSend";
    QByteArray sendline;
      QByteArray sendline1;
    sendline= widget.sendArea->text().toUtf8();
    sendline1=sendline;
    sendline.insert(0,'\x1');
    write(socket_fd, sendline.data(), sendline.size());
 
    widget.ViewTextBrowser->append("me:   "+sendline1.trimmed());
    widget.sendArea->clear();





}

void Client::connectToServer(int port, const char* ip) {
    struct sockaddr_in servaddr;

    socket_fd = socket(AF_INET,SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof servaddr);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET, ip, &(servaddr.sin_addr));
    ::connect(socket_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);
    socketnotifier = new QSocketNotifier(socket_fd, QSocketNotifier::Read);

    QObject::connect(socketnotifier, SIGNAL(activated(int)), this, SLOT(recvData()));
}

void Client::disconnectToServer() {
    ::close(socket_fd);
}
