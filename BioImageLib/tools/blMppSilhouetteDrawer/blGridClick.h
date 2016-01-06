#ifndef blGridClick_H
#define blGridClick_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>

#include <vector>
using namespace std;

#include "blClickPixel.h"
#include <blMpp>

class blGridClick : public QWidget
{
    Q_OBJECT
public:
    explicit blGridClick(int sizeX = 0, int sizeY = 0);

public:
    // setters
    void setRegionSize(int sizeX, int sizeY);
    void setPixelInsideShape(int i, int j);

    // geters
    blMppShapeSilhouette* shapeSilhouette();

    // other
    vector<blGridClick*> generateAllRotations();

public slots:
    void drawGrid();
    void setShapeSilhouette(blMppShapeSilhouette *shapeSilhouette);

private:
    // parameters
    int m_regionSizeX, m_regionSizeY;

    // output
    blMppShapeSilhouette* m_ShapeSilhouette;

    // widgets
    QList<blClickPixel*> m_pixelsLabels;
    QGridLayout* m_gridLayout;

private:
    vector<vector<int> > scaleSilhouetteToGrid(vector<vector<int> > silhouetteSmall);

};

#endif // blGridClick_H
