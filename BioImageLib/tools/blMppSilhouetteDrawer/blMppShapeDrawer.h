#ifndef blMppShapeDrawer_H
#define blMppShapeDrawer_H

#include <QWidget>

#include "blGridClick.h"
#include "blMppShapeGridList.h"

class blMppShapeDrawer : public QWidget
{
    Q_OBJECT
public:
    explicit blMppShapeDrawer(QWidget *parent = 0);
    
signals:
    
public slots:
    void setFile(std::string fileURL);

private slots:
    void addSilhouetteToList();
    void updateSizeX(double size);
    void updateSizeY(double size);
    void resetGrid();
    void generateAllRotations();

private:
    QWidget* createEditor();
    QWidget* createLibraryList();

    blGridClick* m_curentGridClickWidget;
    blMppShapeGridList *m_silhouettelist;
    QHBoxLayout *m_layoutView;

private:
    int m_sizeX;
    int m_sizeY;

    void setCurentWidget(blGridClick* gridClickWidget);
};

#endif // blMppShapeDrawer_H
