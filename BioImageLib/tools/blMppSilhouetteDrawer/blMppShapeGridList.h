#pragma once

#include <QtWidgets>

#include "blClosableWidget.h"
#include "blGridClick.h"
#include "blFlowLayout.h"

class blMppShapeGridList : public QWidget
{
    Q_OBJECT
public:
    explicit blMppShapeGridList(QWidget *parent = 0);
    
signals:
    
public slots:
    void addSilhouette(blGridClick* silhouette);
    QList<blGridClick*> getSilhouettes();
    vector<blMppShapeSilhouette *> getInsideCoords();
    void SaveSilhouettes();
    void SaveSilhouettes(QString fileName);
    void loadSilhouettes(QString fileName);

    void setFile(std::string fileURL);

private slots:
    void removeSilhouette(int removeId);

private:
    QList<blClosableWidget*> m_silhouettes;
    blFlowLayout *m_listLayout;
    QScrollArea *m_scrollArea;
    QWidget *listWidget;
    std::string m_fileURL;

};
