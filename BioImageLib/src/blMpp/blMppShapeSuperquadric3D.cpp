/// \file blMppShapeSuperquadric3D.cpp
/// \brief blMppShapeSuperquadric3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShapeSuperquadric3D.h"
#include "math.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <blGeometry>

blMppShapeSuperquadric3D::blMppShapeSuperquadric3D() : blMppShape3D(){
    m_a = 0;
    m_b = 0;
    m_c = 0;
    m_m = 0;
    m_n = 0;
    m_p = 0;
    m_phi = 0;
    m_theta = 0;
    m_gam = 0;
}

blMppShapeSuperquadric3D::~blMppShapeSuperquadric3D(){

}

std::string blMppShapeSuperquadric3D::name(){
    return "blMppShapeSuperquadric3D";
}

// setter
void blMppShapeSuperquadric3D::setParameters(int a, int b, int c, float m, float n, float p, float phi, float theta,float gamma){
    m_a = a;
    m_b = b;
    m_c = c;
    m_m = m;
    m_n = n;
    m_p = p;
    m_phi = phi;
    m_theta = theta;
    m_gam = gamma;
}

int blMppShapeSuperquadric3D::a(){
    return m_a;
}

int blMppShapeSuperquadric3D::b(){
    return m_b;
}

int blMppShapeSuperquadric3D::c(){
    return m_c;
}

float blMppShapeSuperquadric3D::m(){
    return m_m;
}

float blMppShapeSuperquadric3D::n(){
    return m_n;
}

float blMppShapeSuperquadric3D::p(){
    return m_p;
}

float blMppShapeSuperquadric3D::phi(){
    return m_phi;
}

float blMppShapeSuperquadric3D::theta(){
    return m_theta;
}

float blMppShapeSuperquadric3D::gamma(){
    return m_gam;
}

std::vector<float> blMppShapeSuperquadric3D::getParameters(){
    std::vector<float> params; params.resize(12);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    params[3] = m_a;
    params[4] = m_b;
    params[5] = m_c;
    params[6] = m_m;
    params[7] = m_n;
    params[8] = m_p;
    params[9] = m_phi;
    params[10] = m_theta;
    params[11] = m_gam;
    return params;
}

std::vector<std::string> blMppShapeSuperquadric3D::parametersDescription(){
    std::vector<std::string> params; params.resize(12);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    params[3] = "a";
    params[4] = "b";
    params[5] = "c";
    params[6] = "m";
    params[7] = "n";
    params[8] = "p";
    params[9] = "phi";
    params[10] = "theta";
    params[11] = "gamma";
    return params;
}

void blMppShapeSuperquadric3D::printParam(){

    std::cout << "m_center.x() =" << m_center.x() << std::endl;
    std::cout << "m_center.y() =" << m_center.y()<< std::endl;
    std::cout << "m_center.z() =" << m_center.z()<< std::endl;
    std::cout << "m_a =" << m_a<< std::endl;
    std::cout << "m_b =" << m_b<< std::endl;
    std::cout << "m_c =" << m_c<< std::endl;
    std::cout << "m_m =" << m_m<< std::endl;
    std::cout << "m_n =" << m_n<< std::endl;
    std::cout << "m_p =" << m_p<< std::endl;
    std::cout << "m_phi =" << m_phi<< std::endl;
    std::cout << "m_theta =" << m_theta<< std::endl;
    std::cout << "m_gam =" << m_gam<< std::endl;
}

// silhouette pixels
void blMppShapeSuperquadric3D::computeNormalsBorder(){

    /// \todo from superquadric 2D
    throw blException("computeNormalsBorder not implemented for superquadric 3D");

}



bool blMppShapeSuperquadric3DSortPixels(std::vector<int> i,std::vector<int> j){
    return ( i[0] < j[0] );
}

void blMppShapeSuperquadric3D::getXYZRotationcoefs(float &a, float &b, float &c, float &d, float &e, float &f, float &g, float &h, float &i){

    float cos_phi = cos(m_phi);
    float sin_phi = sin(m_phi);
    float cos_teta = cos(m_theta);
    float sin_teta = sin(m_theta);
    float cos_gam = cos(m_gam);
    float sin_gam = sin(m_gam);

    a = cos_teta*cos_phi;
    b = cos_teta*sin_phi;
    c = -sin_teta;
    d = cos_phi*sin_gam*sin_teta-cos_gam*sin_phi;
    e = sin_phi*sin_gam*sin_teta+cos_phi*cos_gam;
    f = sin_gam*cos_teta;
    g = cos_phi*cos_gam*sin_teta+sin_gam*sin_phi;
    h = sin_phi*cos_gam*sin_teta-sin_gam*cos_phi;
    i = cos_gam*cos_teta;
}

void blMppShapeSuperquadric3D::computeSilhouette(){

    //this->printParam();
    int area = 0;
    m_silhouette = new blMppContainerSilhouette3D();
    int margin;
    if (m_m <= 2 && m_n <= 2 && m_p <= 2){
        margin= m_a + 2;
    }
    else{
        margin = sqrt(m_a*m_a + m_b*m_b + m_c*m_c) + 2;
    }

    float X, Y, Z;
    float value;

    int centerX = m_center.x();
    int centerY = m_center.y();
    int centerZ = m_center.z();

    bool alreadyFoundForX;
    bool alreadyFoundForZ;
    bool p2Unset;
    blPixel point1; blPixel point2;
    int layoutId = -1;
    float a2 = m_a;
    float b2 = m_b;
    float c2 = m_c;
    float xc, yc, zc;

    float a,b,c,d,e,f,g,h,i;
    getXYZRotationcoefs( a, b, c, d, e, f ,g, h, i);

    for (int z = centerZ - margin ; z <= centerZ + margin ; ++z){

        alreadyFoundForZ = false;
        for (int x =centerX - margin ; x <= centerX + margin ; ++x){

            alreadyFoundForX = false;
            p2Unset = true;
            for (int y = centerY - margin ; y <= centerY + margin ; ++y){

                xc = x-centerX; yc = y-centerY; zc = z-centerZ;
                X = a*xc+b*yc+c*zc;
                Y = d*xc+e*yc+f*zc;
                Z = g*xc+h*yc+i*zc;  
                value = pow(fabs(X/a2),m_m) + pow(fabs(Y/b2), m_n) + pow(fabs(Z/c2), m_p);

                if (value <= 1.0){
                    if (!alreadyFoundForZ){
                        m_silhouette->addLayout();
                        layoutId++;
                        alreadyFoundForZ = true;
                    }

                    if (!alreadyFoundForX){
                        point1.setX(x);
                        point1.setY(y);
                        point1.setZ(z);
                        alreadyFoundForX = true;
                    }
                    else{
                        p2Unset = false;
                        point2.setX(x);
                        point2.setY(y);
                        point2.setZ(z);
                    }
                }
            } // end y
            if (alreadyFoundForX){
                if (p2Unset){
                    m_silhouette->addLine(layoutId, blMppContainerSilhouette3DLine(z, x,
                                                                                   blPixel(point1.x(), point1.y(), point1.z()),
                                                                                   blPixel(point1.x(), point1.y(), point1.z()) ));
                    area++;
                }
                else{
                    m_silhouette->addLine(layoutId, blMppContainerSilhouette3DLine(z, x,
                                                                                   blPixel(point1.x(), point1.y(), point1.z()),
                                                                                   blPixel(point2.x(), point2.y(), point2.z()) ));
                    area += abs(point1.y() - point2.y()) +1;
                }
            }
        } // end x
    }
    m_silhouette->setArea(area);

}

void blMppShapeSuperquadric3D::computeMaxDistanceToBarycenter(){
    if (m_m <= 2 && m_n <= 2 && m_p <= 2){
        m_maxDistanceToBarycenter = m_a;
    }
    else{
        m_maxDistanceToBarycenter = sqrt(m_a*m_a + m_b*m_b + m_c*m_c);
    }
}

blMppShape3D *blMppShapeSuperquadric3D::copy(){

    blMppShapeSuperquadric3D* newShape = new blMppShapeSuperquadric3D();
    this->copyData(newShape);
    newShape->setParameters(m_a, m_b, m_c, m_m, m_n, m_p, m_phi, m_theta, m_gam);
    return newShape;
}

