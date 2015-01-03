/// \file blMppShapeCircle.cpp
/// \brief blMppShapeCircle class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014


#include "blMppShapeCircle.h"
#include "math.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <blGeometry>

blMppShapeCircle::blMppShapeCircle() : blMppShape2D(){
    m_radius = 0;
}

blMppShapeCircle::~blMppShapeCircle(){

}

std::string blMppShapeCircle::name(){
    return "blMppShapeCircle";
}

// setter
void blMppShapeCircle::setParameters(int radius){
    m_radius = radius;
}

int blMppShapeCircle::radius(){
    return m_radius;
}

std::vector<float> blMppShapeCircle::getParameters(){
    std::vector<float> params; params.resize(4);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    params[3] = m_radius;
    return params;
}

std::vector<std::string> blMppShapeCircle::parametersDescription(){
    std::vector<std::string> params; params.resize(4);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    params[3] = "radius";
    return params;
}

// silhouette pixels
/*
void blMppShapeSuperquadric2D::computeBorderPixels(int borderWidth){

}
*/

void blMppShapeCircle::computeNormalsBorder(){

    int cx = m_center.x();
    int cy = m_center.y();
    float nx, ny;
    for (int i = 0 ; i < m_borderPixels->size() ; ++i){
        blPixel curentPixel = m_borderPixels->operator [](i);
        nx = curentPixel.x() - cx;
        ny = curentPixel.y() - cy;
        float norm = sqrt(nx*nx + ny*ny);
        nx /= norm;
        ny /= norm;

        std::vector<float> normal; normal.resize(2);
        normal[0] = nx;
        normal[1] = ny;

        m_normalsBorder->operator [](i) = normal;
    }
    m_normalsCalculated = true;

}



bool blMppShapeCircleSortPixels(std::vector<int> i,std::vector<int> j){
    return ( i[0] < j[0] );
}

void blMppShapeCircle::computeSilhouette(){

    std::vector<std::vector<int> > border;
    blMathGeometry::CalculateCircleCoordinates( m_center.x(), m_center.y(), m_radius, border);


    std::sort(border.begin(), border.end(), blMppShapeCircleSortPixels);

    int area = 0;
    m_silhouette = new blMppContainerSilhouette();
    std::vector<bool> alreadyUse; alreadyUse.resize(border.size());
    for (int i = 0 ; i < alreadyUse.size() ; ++i){
        alreadyUse[i] = false;
    }
    std::vector<int> pos;
    float x; float y1, y2, yt;
    for (int i = 0 ; i < border.size() ; ++i){
        if (!alreadyUse[i]){
            x = border[i][0];
            pos.clear();
            for (int j = i ; j < border.size() ; ++j){
                if (border[j][0] == x){
                    pos.push_back(j);
                }
            }


            y1 = border[ pos[0] ][1];
            y2 = border[ pos[0] ][1];
            for (int m = 1 ; m < pos.size() ; ++m){
                if (border[pos[m] ][1] > y2){
                    y2 = border[pos[m] ][1];
                }
                if (border[pos[m] ][1] < y1){
                    y1 = border[pos[m] ][1];
                }
            }

            if (pos.size() > 1){
                if (y1 > y2){
                    yt = y2;
                    y2 = y1;
                    y1 = yt;
                }

                m_silhouette->addLine(blMppContainerSilhouetteLine(x,
                                                                   blPixel(x, y1),
                                                                   blPixel(x, y2)));
                area += abs(y1 - y2) +1;
                for (int k = 0 ; k < pos.size() ; ++k){
                    alreadyUse[ pos[k] ] = true;
                }
            }
            else{
                m_silhouette->addLine(blMppContainerSilhouetteLine(x,
                                                                   blPixel(x, border[i][1]),
                                      blPixel(x, border[i][1])));
                area++;
            }
            alreadyUse[i] = true;
        }
    }
    //std::cout << "area = " << area << std::endl;
    m_silhouette->setArea(area);
    //m_silhouette->printConsole();
}

void blMppShapeCircle::computeMaxDistanceToBarycenter(){
    m_maxDistanceToBarycenter = m_radius;
}

blMppShape2D *blMppShapeCircle::copy(){

    blMppShapeCircle* newShape = new blMppShapeCircle();
    this->copyData(newShape);
    newShape->setParameters(m_radius);
    return newShape;
}

