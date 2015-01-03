/// \file blMppShapeEllipse.cpp
/// \brief blMppShapeEllipse class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShapeEllipse.h"
#include "math.h"
#include <iostream>
#include <sstream>
#include <fstream>

blMppShapeEllipse::blMppShapeEllipse() : blMppShape2D(){
    m_a = 0;
    m_b = 0;
    m_theta = 0;
}

blMppShapeEllipse::~blMppShapeEllipse(){

}

std::string blMppShapeEllipse::name(){
    return "blMppShapeEllipse";
}

// setter
void blMppShapeEllipse::setParameters(int a, int b, float theta){
    m_a = a;
    m_b = b;
    m_theta = theta;
}

int blMppShapeEllipse::a(){
    return m_a;
}

int blMppShapeEllipse::b(){
    return m_b;
}

float blMppShapeEllipse::theta(){
    return m_theta;
}

std::vector<float> blMppShapeEllipse::getParameters(){
    std::vector<float> params; params.resize(6);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    params[3] = m_a;
    params[4] = m_b;
    params[5] = m_theta;
    return params;
}

std::vector<std::string> blMppShapeEllipse::parametersDescription(){
    std::vector<std::string> params; params.resize(6);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    params[3] = "a";
    params[4] = "b";
    params[5] = "theta";
    return params;
}

void blMppShapeEllipse::computeNormalsBorder(){

    // compute A
    float inva = 1.0/float(m_a);
    float invb = 1.0/float(m_b);
    float cosi = cos(m_theta);
    float sini = sin(m_theta);

    m_normalsBorder = new std::vector<std::vector<float> >();
    m_normalsBorder->resize(m_borderPixels->size());
    float X,Y,Xt,Yt,Fx,Fy, nx, ny, norm ;
    for (int i = 0 ; i < m_borderPixels->size() ; ++i){
        blPixel curentPixel = m_borderPixels->operator [](i);
        X = curentPixel.x() - m_center.x();
        Y = curentPixel.y() - m_center.y();

        Xt = inva*(X*cosi + Y*sini);
        Yt = invb*(-X*sini + Y*cosi);

        if (Xt < 0){
            Fx = -2*pow(fabs(Xt), 2-1);
        }
        else{
            Fx = 2*pow(fabs(Xt), 2-1);
        }
        if (Yt < 0){
            Fy = -2*pow(fabs(Yt), 2-1);
        }
        else{
            Fy = 2 *pow(fabs(Yt), 2-1);
        }

        nx = inva*Fx*cosi - invb*Fy*sini;
        ny = inva*Fx*sini + invb*Fy*cosi;
        norm = sqrt(nx*nx + ny*ny);
        nx /= norm;
        ny /= norm;

        std::vector<float> normal; normal.resize(2);
        normal[0] = nx;
        normal[1] = ny;

        m_normalsBorder->operator [](i) = normal;
    }
    m_normalsCalculated = true;

}

void blMppShapeEllipse::computeSilhouette(){

    int area = 0;
    m_silhouette = new blMppContainerSilhouette();
    float a = m_a;
    float b = m_b;

    int margin;
    margin = a + 2;

    float X, Y;
    float value;
    float cosi = cos(m_theta);
    float sinu = sin(m_theta);
    float a2 = a, b2 = b;

    int centerX = m_center.x();
    int centerY = m_center.y();

    bool alreadyFoundForX;
    bool p2Unset;
    blPixel point1; blPixel point2;
    for (int x =centerX - margin ; x <= centerX + margin ; ++x){
        alreadyFoundForX = false;
        p2Unset = true;
        for (int y = centerY - margin ; y <= centerY + margin ; ++y){
            X = (x - centerX)*cosi + (y - centerY)*sinu;
            Y = -(x - centerX)*sinu + (y - centerY)*cosi;
            value = pow((X/a2),2) + pow((Y/b2), 2);
            if (value < 1.0){
                if (!alreadyFoundForX){
                    point1.setX(x);
                    point1.setY(y);
                    alreadyFoundForX = true;
                }
                else{
                    p2Unset = false;
                    point2.setX(x);
                    point2.setY(y);
                }
            }
        } // end y
        if (alreadyFoundForX){
            if (p2Unset){
                m_silhouette->addLine(blMppContainerSilhouetteLine(x,
                                                                   blPixel(point1.x(), point1.y()),
                                                                   blPixel(point1.x(), point1.y()) ));
                area++;
            }
            else{
                m_silhouette->addLine(blMppContainerSilhouetteLine(x,
                                                                   blPixel(point1.x(), point1.y()),
                                                                   blPixel(point2.x(), point2.y()) ));
                area += abs(point1.y() - point2.y()) +1;
            }
        }
    } // end x
    m_silhouette->setArea(area);
}


void blMppShapeEllipse::computeMaxDistanceToBarycenter(){
    m_maxDistanceToBarycenter = 2.0*m_a;
}

blMppShape2D *blMppShapeEllipse::copy(){

    blMppShapeEllipse* newShape = new blMppShapeEllipse();
    this->copyData(newShape);
    newShape->setParameters(m_a, m_b, m_theta);
    return newShape;

}
