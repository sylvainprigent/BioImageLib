#include "blGridClick.h"
#include <QGridLayout>
#include <QLabel>
#include <math.h>
#include <QDebug>

blGridClick::blGridClick(int sizeX, int sizeY){

    setRegionSize(sizeX, sizeY);
    m_gridLayout = new QGridLayout();
    m_gridLayout->setSpacing(1);
    m_gridLayout->setContentsMargins(0,0,0,0);
    this->setStyleSheet("QWidget{background-color: black;}");
    drawGrid();
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_ShapeSilhouette = new blMppShapeSilhouette();
}

void blGridClick::setRegionSize(int sizeX, int sizeY){
    m_regionSizeX = sizeX;
    m_regionSizeY = sizeY;
}

void blGridClick::drawGrid(){

    // 0- Free previous
    QLayoutItem* item;
    while ( ( item = m_gridLayout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }

    m_pixelsLabels.clear();

    // 1- Calculate central pixel
    int centerX = m_regionSizeX/2;
    int centerY = m_regionSizeY/2;

    // 2- plot the grid
    for (int i = 0 ; i < m_regionSizeX ; ++i){
        for (int j = 0 ; j < m_regionSizeY ; ++j){
            blClickPixel* label = new blClickPixel(i,j,false);
            if (i == centerX && j == centerY){
                label->setActivatedBlack(true);
                label->setEditable(false);
            }
            m_pixelsLabels.append(label);
            m_gridLayout->addWidget(label,i,j);
        }
    }
    this->setLayout(m_gridLayout);
    this->update();
}

blMppShapeSilhouette*  blGridClick::shapeSilhouette(){

    int centerX = m_regionSizeX/2;
    int centerY = m_regionSizeY/2;

    vector<blPixel>* insidePixels = new vector<blPixel>();
    int x,y;
    for (int i = 0 ; i < m_pixelsLabels.size() ; ++i){
        if (m_pixelsLabels.at(i)->isActivatedBlack()){
            x = m_pixelsLabels.at(i)->getPosX() - centerX;
            y = m_pixelsLabels.at(i)->getPosY() - centerY;
            insidePixels->push_back(blPixel(x,y));
        }
    }

    // setSilhouette
    m_ShapeSilhouette->setInsidePixels(insidePixels);
    return m_ShapeSilhouette;
}

void blGridClick::setShapeSilhouette(blMppShapeSilhouette *shapeSilhouette){

    blMppContainerSilhouette* curentSilhouette = shapeSilhouette->silhouette();
    std::vector<blPixel> *insidePixels = curentSilhouette->insidePixels();

    // calculate width and height of the silhouette
    int height = 0;
    int width = 0;
    for (unsigned int p = 0 ; p < insidePixels->size() ; ++p){
        blPixel curentP = insidePixels->at(p);
        if (abs(curentP.x()) > height){
            height = abs(curentP.x());
        }
        if (abs(curentP.y()) > width){
            width = abs(curentP.y());
        }
    }
    //width +=1;
    //height += 1;
    m_regionSizeX = 2*height + 4;
    m_regionSizeY = 2*width + 4;

    // 1- Calculate central pixel
    int centerX = width +1;
    int centerY = height +1;

    for (int i = 0 ; i < m_regionSizeX ; ++i){
        for (int j = 0 ; j < m_regionSizeY ; ++j){
            blClickPixel* label = new blClickPixel(i,j,false);
            //if (i == centerX && j == centerY){
            //    label->setActivatedBlack(true);
            //    label->setEditable(false);
            //}
            for(int p = 0 ; p < insidePixels->size() ; ++p){
                blPixel curentP = insidePixels->at(p);
                if ( curentP.x() + centerX +1 == i && curentP.y() + centerY == j ){
                     label->setActivatedBlack(true);
                     break;
                }
            }
            m_pixelsLabels.append(label);
            m_gridLayout->addWidget(label,i,j);
        }
    }
    this->setLayout(m_gridLayout);
    this->update();
}

vector<blGridClick*> blGridClick::generateAllRotations(){

    //generateSilhouetteBorderByDilatation();
    vector<blGridClick*> allRotations;

    //m_pixels = getInsideShape();

    int minX = 0;
    int minY = 0;
    vector<vector<int> > silhouetteSmall = shapeSilhouette()->generateInsideShapeImage(minX,minY);

    vector<vector<int> > silhouette;
    silhouette = scaleSilhouetteToGrid(silhouetteSmall);

    for (float teta = 0 ; teta < 2*3.14 ; teta+=0.1){
        vector<int> px1;
        vector<int> py1;

        int xc = m_regionSizeX/2;
        int yc = m_regionSizeY/2;
        // Rotate the points
        float cosi = cos(teta);
        float sinu = sin(teta);
        float xr, yr, G, dx, dy;
        for (int i=0 ; i < m_regionSizeX ; ++i){
            for (int j=0 ; j < m_regionSizeY ; ++j){
                xr = float(i-xc)*cosi - float(j-yc)*sinu;
                yr = float(i-xc)*sinu + float(j-yc)*cosi;
                xr = xr + xc;
                yr = yr + yc;
                if (floor(xr) >= 0 && floor(yr) >= 0 && floor(xr)+1 < m_regionSizeX && floor(yr)+1 < m_regionSizeY ){
                    dx = xr - floor(xr);
                    dy = yr - floor(yr);

                    G = (1-dx) * (1-dy) * silhouette[floor(xr)][floor(yr)] +
                            (  dx) * (1-dy) * silhouette[floor(xr)+1][floor(yr)] +
                            (1-dx) * (  dy) * silhouette[floor(xr)][floor(yr)+1] +
                            (  dx) * (  dy) * silhouette[floor(xr)+1][floor(yr)+1];

                    if ( G >= 0.5) {
                        px1.push_back(i);
                        py1.push_back(j);
                    }
                }
            }
        }
        // Create the grid object
        blGridClick *grid = new blGridClick(m_regionSizeX,m_regionSizeY);
        for (unsigned int l = 0 ; l < px1.size() ; ++l){
            grid->setPixelInsideShape(px1[l], py1[l]);
        }

        // keep it only if the silhouette doesn't exists
        vector<vector<int> > silhouetteCurentSmall = grid->shapeSilhouette()->generateInsideShapeImage(minX,minY);
        vector<vector<int> > silhouetteCurent;
        silhouetteCurent = scaleSilhouetteToGrid(silhouetteCurentSmall);

        bool exists = false;
        for (unsigned int obj = 0 ; obj < allRotations.size() ; ++obj){
            vector<vector<int> > silhouette = scaleSilhouetteToGrid(allRotations.at(obj)->shapeSilhouette()->generateInsideShapeImage(minX,minY));
            int count = 0;
            for (int i=0 ; i < m_regionSizeX ; ++i){
                for (int j=0 ; j < m_regionSizeY ; ++j){
                    //std::cout << "i= " << i << ", j = " << j << std::endl;
                    if (silhouetteCurent[i][j] == silhouette[i][j]){
                        count++;
                    }
                }
            }
            if (count == m_regionSizeX*m_regionSizeY){
                exists = true;
                break;
            }
        }

        // generate the border
        if (!exists){
            allRotations.push_back(grid);
        }
    }
    return allRotations;
}

vector<vector<int> > blGridClick::scaleSilhouetteToGrid(vector<vector<int> > silhouetteSmall){

    vector<vector<int> > silhouette; silhouette.resize(m_regionSizeX);
    for (int i = 0 ; i < m_regionSizeX ; ++i){
        silhouette[i].resize(m_regionSizeY);
        for (int j = 0 ; j < m_regionSizeY ; ++j){
            silhouette[i][j] = 0;
        }
    }
    // copy solhouette small
    int addLine = m_regionSizeX/2 - silhouetteSmall.size()/2;
    int addCol = m_regionSizeY/2 - silhouetteSmall[0].size()/2;
    for (int i = 0 ; i < silhouetteSmall.size() ; ++i){
        for (int j = 0 ; j  < silhouetteSmall[i].size() ; ++j){
            silhouette[i + addLine][j + addCol] = silhouetteSmall[i][j];
        }
    }
    return silhouette;
}

void blGridClick::setPixelInsideShape(int i, int j){
    for (int l = 0 ; l < m_pixelsLabels.size() ; ++l){
        if (m_pixelsLabels.at(l)->getPosX() == i && m_pixelsLabels.at(l)->getPosY() == j){
            m_pixelsLabels.at(l)->setActivatedBlack(true);
        }
    }
}
