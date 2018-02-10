/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.h
 * Author: nelly
 *
 * Created on December 4, 2016, 11:29 PM
 */

#ifndef _CLIENT_H
#define _CLIENT_H


#include "ui_Client.h"

class QSocketNotifier;

class Client : public QDialog {
    Q_OBJECT
    public:
        Client();
        virtual ~Client();
        void connectToServer(int port, const char* ip);
        void disconnectToServer();

    public slots:
        void textSend(void);
        void sendFile(void);
        void recvText(void);
        void recvData(void);

        
private:
    Ui::Client widget;
    int socket_fd;
    QSocketNotifier* socketnotifier;
};

#endif /* _CLIENT_H */
