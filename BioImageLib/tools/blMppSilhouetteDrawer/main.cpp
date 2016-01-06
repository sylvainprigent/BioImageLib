#include <QApplication>

#include <iostream>

#include "blGridClick.h"
#include "blMppShapeDrawer.h"
#include "blClosableWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string file;
    if (argc > 1){
        file = argv[1];
    }

    std::cout << "file = " << file << std::endl;

    blMppShapeDrawer w;
    w.setFile(file);
    w.show();
    
    QString styleFile = "C:\\Users\\sprigent\\Documents\\codes\\BioImageLab\\theme\\default\\stylesheet.css";
    a.setStyleSheet("file:///" + styleFile);


    return a.exec();
}
