/// \file blMppShapeSuperquadric2D.cpp
/// \brief blMppShapeSuperquadric2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShapeSuperquadric2D.h"
#include "math.h"
#include <iostream>
#include <sstream>
#include <fstream>

blMppShapeSuperquadric2D::blMppShapeSuperquadric2D() : blMppShape2D(){
    m_a = 0;
    m_b = 0;
    m_m = 0;
    m_n = 0;
    m_theta = 0;
}

blMppShapeSuperquadric2D::~blMppShapeSuperquadric2D(){

}

std::string blMppShapeSuperquadric2D::name(){
    return "blMppShapeSuperquadric2D";
}

// setter
void blMppShapeSuperquadric2D::setParameters(int a, int b, float m, float n, float theta){
    m_a = a;
    m_b = b;
    m_m = m;
    m_n = n;
    m_theta = theta;
}

int blMppShapeSuperquadric2D::a(){
    return m_a;
}

int blMppShapeSuperquadric2D::b(){
    return m_b;
}

float blMppShapeSuperquadric2D::m(){
    return m_m;
}

float blMppShapeSuperquadric2D::n(){
    return m_n;
}

float blMppShapeSuperquadric2D::theta(){
    return m_theta;
}

std::vector<float> blMppShapeSuperquadric2D::getParameters(){
    std::vector<float> params; params.resize(8);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    params[3] = m_a;
    params[4] = m_b;
    params[5] = m_m;
    params[6] = m_n;
    params[7] = m_theta;
    return params;
}

std::vector<std::string> blMppShapeSuperquadric2D::parametersDescription(){
    std::vector<std::string> params; params.resize(8);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    params[3] = "a";
    params[4] = "b";
    params[5] = "m";
    params[6] = "n";
    params[7] = "theta";
    return params;
}

void blMppShapeSuperquadric2D::computeNormalsBorder(){

    //std::cout << "blMppShapeSuperquadric2D::computeNormalsBorder begin" << std::endl;
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
            Fx = -m_m*pow(fabs(Xt), m_m-1);
        }
        else{
            Fx = m_m*pow(fabs(Xt), m_m-1);
        }
        if (Yt < 0){
            Fy = -m_n*pow(fabs(Yt), m_n-1);
        }
        else{
            Fy = m_n*pow(fabs(Yt), m_n-1);
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
        //if (i==0)
        //    std::cout << "nx l =" << m_normals->operator [](i)[0] << "ny l = " << m_normals->operator [](i)[1] << std::endl;
    }
    m_normalsCalculated = true;

}

void blMppShapeSuperquadric2D::computeSilhouette(){

    int area = 0;
    m_silhouette = new blMppContainerSilhouette();
    float a = m_a;
    float b = m_b;

    int margin;
    if (m_m > 4 || m_n > 4){
        margin = int(sqrt(a*a + b*b))+ 2;
    }
    else{
        margin = a + 2;
    }

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
            value = pow(fabs(X/a2),m_m) + pow(fabs(Y/b2), m_n);
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


void blMppShapeSuperquadric2D::computeMaxDistanceToBarycenter(){
    m_maxDistanceToBarycenter = sqrt(m_a*m_a + m_b*m_b);
}

blMppShapeSuperquadric2D *blMppShapeSuperquadric2D::copy(){

    blMppShapeSuperquadric2D* newShape = new blMppShapeSuperquadric2D();
    this->copyData(newShape);
    newShape->setParameters(m_a, m_b, m_m, m_n, m_theta);
    return newShape;

}
