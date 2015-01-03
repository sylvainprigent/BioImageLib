/// \file blMppShapeSphere.cpp
/// \brief blMppShapeSphere class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShapeSphere.h"
#include "math.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <blGeometry>

blMppShapeSphere::blMppShapeSphere() : blMppShape3D(){
    m_radius = 0;
}

blMppShapeSphere::~blMppShapeSphere(){

}

std::string blMppShapeSphere::name(){
    return "blMppShapeSphere";
}

// setter
void blMppShapeSphere::setParameters(int radius){
    m_radius = radius;
}

int blMppShapeSphere::radius(){
    return m_radius;
}

std::vector<float> blMppShapeSphere::getParameters(){
    std::vector<float> params; params.resize(4);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    params[3] = m_radius;
    return params;
}

std::vector<std::string> blMppShapeSphere::parametersDescription(){
    std::vector<std::string> params; params.resize(4);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    params[3] = "radius";
    return params;
}

// silhouette pixels
void blMppShapeSphere::computeNormalsBorder(){

    int cx = m_center.x();
    int cy = m_center.y();
    int cz = m_center.z();
    float nx, ny, nz;
    m_normalsBorder = new std::vector<std::vector<float> >();
    m_normalsBorder->resize(m_borderPixels->size());
    for (int i = 0 ; i < m_borderPixels->size() ; ++i){
        blPixel curentPixel = m_borderPixels->operator [](i);
        nx = curentPixel.x() - cx;
        ny = curentPixel.y() - cy;
        nz = curentPixel.z() - cz;
        float norm = sqrt(nx*nx + ny*ny + nz*nz);
        nx /= norm;
        ny /= norm;
        nz /= norm;

        std::vector<float> normal; normal.resize(3);
        normal[0] = nx;
        normal[1] = ny;
        normal[2] = nz;

        m_normalsBorder->operator [](i) = normal;
    }
    m_normalsCalculated = true;

}



bool blMppShapeSphereSortPixels(std::vector<int> i,std::vector<int> j){
    return ( i[0] < j[0] );
}

void blMppShapeSphere::computeSilhouette(){

    int area = 0;
    m_silhouette = new blMppContainerSilhouette3D();
    int margin = m_radius + 2;

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
    float r2 = m_radius*m_radius;
    for (int z = centerZ - margin ; z <= centerZ + margin ; ++z){

        alreadyFoundForZ = false;
        for (int x =centerX - margin ; x <= centerX + margin ; ++x){

            alreadyFoundForX = false;
            p2Unset = true;
            for (int y = centerY - margin ; y <= centerY + margin ; ++y){

                X = (x - centerX);
                Y = (y - centerY);
                Z = (z - centerZ);
                value = X*X + Y*Y + Z*Z;
                if (value <= r2){
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

void blMppShapeSphere::computeMaxDistanceToBarycenter(){
    m_maxDistanceToBarycenter = m_radius;
}

blMppShape3D *blMppShapeSphere::copy(){

    blMppShapeSphere* newShape = new blMppShapeSphere();
    this->copyData(newShape);
    newShape->setParameters(m_radius);
    return newShape;
}

