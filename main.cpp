/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: nelly
 *
 * Created on December 4, 2016, 10:57 PM
 */

#include <QApplication>
#include "Client.h"
int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    // create and show your widgets here
    Client form;
    form.show();
    
    return app.exec();
}
