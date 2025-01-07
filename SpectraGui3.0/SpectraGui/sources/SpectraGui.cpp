// CMakeProject1.cpp: define el punto de entrada de la aplicación.
//

#include "headers/mainwindow.h"
#include <QPushButton>
#include <QApplication>
#include "importantFiles/compilationType.h"
#include <QIcon>
#include <iostream>
using namespace std;


int main(int argc, char **argv)
{
	
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/sicon.png"));
    mainw* m = new mainw();
	m->grabGesture(Qt::PanGesture);
	m->grabGesture(Qt::PinchGesture);
	m->show();
    return app.exec();
	
}
