#include "blMppShapeGridList.h"

#include <QtGui>

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

blMppShapeGridList::blMppShapeGridList(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    m_scrollArea = new QScrollArea();
    listWidget = new QWidget;
    m_listLayout = new blFlowLayout;
    m_listLayout->setContentsMargins(0,0,0,0);
    listWidget->setLayout(m_listLayout);

    m_scrollArea->setObjectName("KhcDropBarScroll");
    //m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setAlignment(Qt::AlignCenter);
    m_scrollArea->setWidget(listWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setContentsMargins(0,0,0,0);

    layout->addWidget(m_scrollArea);

    // buttons
    QPushButton *saveButton = new QPushButton(tr("Save"));
    saveButton->setObjectName("btnDefault");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(SaveSilhouettes()));
    layout->addWidget(saveButton, 0, Qt::AlignRight);

    this->setLayout(layout);

}

void blMppShapeGridList::addSilhouette(blGridClick* silhouette){

    // create the closable widget
    blClosableWidget *closable = new blClosableWidget(silhouette);
    closable->setId(m_silhouettes.size());
    m_silhouettes.append(closable);
    connect(closable, SIGNAL(CloseMe(int)), this, SLOT(removeSilhouette(int)));

    // add to list
    m_listLayout->addWidget(closable);
    m_scrollArea->setWidget(listWidget);
}

QList<blGridClick*> blMppShapeGridList::getSilhouettes(){
    /// \todo implement this method
    QList<blGridClick*> data;
    return data;
}

vector<blMppShapeSilhouette*> blMppShapeGridList::getInsideCoords(){

    vector<blMppShapeSilhouette*> silhouettes;
    for (int i = 0 ; i < m_silhouettes.size() ; ++i){
        blGridClick* widget = (blGridClick*)m_silhouettes.at(i)->GetWidget();
        silhouettes.push_back(widget->shapeSilhouette());
    }
    return silhouettes;
}

void blMppShapeGridList::setFile(std::string fileURL){
     m_fileURL = fileURL;

     QFile file(QString::fromStdString(fileURL));
     if(file.exists()){
         this->loadSilhouettes(QString::fromStdString(fileURL));
     }
}

void blMppShapeGridList::SaveSilhouettes(){
    QString fileName;
    if (m_fileURL == ""){
        fileName = QFileDialog::getSaveFileName(this, "Save file", "", ".txt");
    }
    else{
        fileName = QString::fromStdString(m_fileURL);
    }
    SaveSilhouettes(fileName);
}


void blMppShapeGridList::loadSilhouettes(QString fileName){

    blMppGeneratorShapeSilhouetteTemplate2D shapegenerator;
    shapegenerator.setSilhouetteLibraryFile(fileName.toStdString());
    shapegenerator.initialize();
    std::vector<blMppShapeSilhouette*> silhouettes = shapegenerator.shapesLibrary();

    m_silhouettes.clear();
    for (int i = 0 ; i < silhouettes.size() ; ++i )
    {
        blGridClick* silhouetteGrid = new blGridClick();
        silhouetteGrid->setShapeSilhouette(silhouettes[i]);
        addSilhouette(silhouetteGrid);
    }
}

void blMppShapeGridList::SaveSilhouettes(QString fileName){

    // get silhouettes
    vector<blMppShapeSilhouette*> silhouettes = getInsideCoords();

    // save silhouettes to file
    // 1- Open file
    ofstream file(fileName.toStdString().c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < silhouettes.size() ; ++i){

            file << "object " << i << " silhouette" << endl;
            silhouettes[i]->computeSilhouette();
            blMppContainerSilhouette* curentSilhouette = silhouettes[i]->silhouette();
            std::vector<blMppContainerSilhouetteLine> *data = curentSilhouette->data();
            for (unsigned int line = 0 ; line < data->size() ; ++line){
                file << "object " << i << " new line" << endl;
                for (int pair = 0 ; pair < data->operator [](line).pairNumber(); ++pair){
                    // add pair
                    blPixel left = data->operator [](line).getPixelLeftAt(pair);
                    file << left.x() << "\t" << left.y() << "\t" << left.z() << endl;
                    blPixel right = data->operator [](line).getPixelRightAt(pair);
                    file << right.x() << "\t" << right.y() << "\t" << right.z() << endl;
                }
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName.toStdString() << endl;
    }
}

void blMppShapeGridList::removeSilhouette(int removeId){

    if (removeId < m_silhouettes.size()-1){
        for (int i = removeId+1 ; i < m_silhouettes.size() ; ++i){
            m_silhouettes.at(i)->setId(i-1);
        }
    }
    m_silhouettes.at(removeId)->hide();
    m_listLayout->removeWidget(m_silhouettes.at(removeId));
    //delete m_silhouettes.at(removeId);
    m_silhouettes.removeAt(removeId);
}
