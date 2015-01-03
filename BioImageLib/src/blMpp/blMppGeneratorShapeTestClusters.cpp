/// \file blMppGeneratorShapeTestClusters.cpp
/// \brief blMppGeneratorShapeTestClusters class
/// \author Emmanuel Soubies (Emmanuel.Soubies@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeTestClusters.h"

#include <blMath>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

blMppGeneratorShapeTestClusters::blMppGeneratorShapeTestClusters() : blMppGeneratorShape(){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeTestClusters::blMppGeneratorShapeTestClusters(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeTestClusters::blMppGeneratorShapeTestClusters(blImageSize size)
    : blMppGeneratorShape(size){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeTestClusters::~blMppGeneratorShapeTestClusters(){

}

// blProcess methods
std::string blMppGeneratorShapeTestClusters::name(){
    return "blMppGeneratorShapeTestClusters";
}

void blMppGeneratorShapeTestClusters::initParametersWithDefaultsValues(){
    m_fileUrl = "";
    m_shapeCounter = 0;
    m_maxSize = 0;
}

// setters parameters
void blMppGeneratorShapeTestClusters::setShapeFile(std::string fileUrl){
    m_fileUrl = fileUrl;
}

// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeTestClusters::initialize(){

    m_shapeCounter = 0;
    // open file
    ifstream file(m_fileUrl.c_str(), ios::in);
    int line = -1;

    int maxR = 0;
    vector<float> Shape; Shape.resize(3);
    if(file)
    {
        while (!file.eof()){
            line++;
            for (int j = 0 ; j < 3 ; ++j){
                file >> Shape[j];
            }
            if (Shape[2] > maxR){
                maxR = Shape[2];
            }
            m_Shapes.push_back(Shape);
        }
        file.close();
    }
    else{
        cerr << "IO::LoadShapeFromTxtFile -> Impossible to open the file " << m_fileUrl << "!" << endl;
    }

    m_maxSize = 2*maxR;
}

blMppShape* blMppGeneratorShapeTestClusters::generateRandomUsingMask(){
    return NULL;
}

blMppShape* blMppGeneratorShapeTestClusters::generateRandomUsingArea(){


    vector<float> shapeData = m_Shapes[m_shapeCounter];
    m_shapeCounter++;

    blMppShapeSuperquadric2D* shape = new blMppShapeSuperquadric2D;
    shape->setCenter(blPixel(shapeData[0], shapeData[1]));
    shape->setParameters(shapeData[2], shapeData[2], 2, 2, 0);

    // init shape
    shape->computeSilhouette();
    std::cout << "calculate border pixels" << std::endl;
    shape->computeBorderPixels(m_borderWidth);

    std::vector<blPixel>* translated = new std::vector<blPixel>();
    translatePixels(shape->borderPixels(), blPixel(shapeData[0], shapeData[1]),  translated);
    shape->setBorderPixels(translated);

    shape->computeNormalsBorder();


    return shape;

}


int blMppGeneratorShapeTestClusters::shapesMaxLength(){
    return m_maxSize;
}

// virtual
blMppShape* blMppGeneratorShapeTestClusters::translate(blMppShape* shape){

    return NULL;
}

blMppShape* blMppGeneratorShapeTestClusters::rotate(blMppShape* shape){

    return NULL;
}

blMppShape* blMppGeneratorShapeTestClusters::resize(blMppShape* shape){

    return NULL;
}

void blMppGeneratorShapeTestClusters::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y());
    }
}
