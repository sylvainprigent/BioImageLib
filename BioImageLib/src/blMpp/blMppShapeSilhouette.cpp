/// \file blMppShapeSilhouette.cpp
/// \brief blMppShapeSilhouette class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShapeSilhouette.h"
#include "math.h"
#include <iostream>
#include <sstream>
#include <fstream>

blMppShapeSilhouette::blMppShapeSilhouette() : blMppShape2D(){
    m_position = -1;
}

blMppShapeSilhouette::~blMppShapeSilhouette(){

}

std::string blMppShapeSilhouette::name(){
    return "blMppShapeSilhouette";
}

// setter
void blMppShapeSilhouette::setParameters(int pos){
    m_position = pos;
}

int blMppShapeSilhouette::position(){
    return m_position;
}

std::vector<float> blMppShapeSilhouette::getParameters(){
    std::vector<float> params; params.resize(4);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    params[3] = m_position;
    return params;
}

std::vector<std::string> blMppShapeSilhouette::parametersDescription(){
    std::vector<std::string> params; params.resize(8);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    params[3] = "position";
    return params;
}

void blMppShapeSilhouette::computeSilhouette(){

    if (!m_insidePixelsCalculated){
        throw blException("blMppShapeSilhouette: the inside pixels are needed to calculate the silhouette");
    }
    // get the silhouette area
    int maxX = -999999;
    int minX = 999999;
    int maxY = -999999;
    int minY = 999999;
    int x, y;
    for (int i = 0 ; i < m_insidePixels->size() ; ++i ){
        x = m_insidePixels->operator [](i).x();
        y = m_insidePixels->operator [](i).y();
        if (x > maxX) {maxX = x;}
        if (x < minX) {minX = x;}
        if (y > maxY) {maxY = y;}
        if (y < minY) {minY = y;}
    }
    m_maxDistanceToBarycenter = sqrt( maxX*maxX + maxY*maxY);
    int sizeX = maxX+1 - (minX-1) +1;
    int sizeY = maxY+1 - (minY-1) +1;

    std::vector<std::vector<int> > imageSilhouette; imageSilhouette.resize(sizeX);
    for (int i=0 ; i < sizeX ; ++i){
        imageSilhouette[i].resize(sizeY);
        for (int j=0 ; j < sizeY ; ++j){
            imageSilhouette[i][j] = 0;
        }
    }
    for (int i = 0 ; i < m_insidePixels->size() ; ++i ){
        x = m_insidePixels->operator [](i).x();
        y = m_insidePixels->operator [](i).y();
        imageSilhouette[x-(minX-1)][y-(minY-1)] = 1;
    }

    /*
    for (int i = 0 ; i < sizeX ; ++i){
        for (int j = 0 ; j < sizeY ; ++j){
            std::cout << imageSilhouette[i][j] << "\t" ;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    */

    // get the points pairs
    int previous = 0;
    int value;
    blPixel startingPair;
    bool foundFinish = false;
    m_silhouette = new blMppContainerSilhouette();
    std::cout << "image silhouette size = " << imageSilhouette.size() << std::endl;
    for (int i = 0 ; i < sizeX ; ++i){
        blMppContainerSilhouetteLine line = blMppContainerSilhouetteLine(i - (sizeX/2));
        for (int j = 0 ; j < sizeY ; ++j){
            value = imageSilhouette[i][j];
            //std::cout << "value = " << value << std::endl;
            if ( value == 1 && previous == 0 ){
                // start line
                startingPair = blPixel(i - (sizeX/2), j - (sizeY/2));
            }
            if ( value == 0 && previous == 1 ){
                // finich line
                foundFinish = true;
                line.addPointPair(startingPair, blPixel(i - (sizeX/2), j - (sizeY/2)-1));

            }
            if (foundFinish == true){
                //std::cout << "add line " << std::endl;
                m_silhouette->addLine(line);
                foundFinish = false;
            }
            previous = value;
        }
    }
}


void blMppShapeSilhouette::computeMaxDistanceToBarycenter(){
    // alrady calculated during the silhouette calculation
}

blMppShape2D *blMppShapeSilhouette::copy(){

    blMppShapeSilhouette* newShape = new blMppShapeSilhouette();
    this->copyData(newShape);
    newShape->setParameters(m_position);
    return newShape;
}
