/// \file blMppGeneratorShapeCircleIn3D.cpp
/// \brief blMppGeneratorShapeCircleIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeCircleIn3D.h"
#include <blMath>
#include <blRand/blRandomGeneratorPM.h>

using namespace std;

blMppGeneratorShapeCircleIn3D::blMppGeneratorShapeCircleIn3D() : blMppGeneratorShapeCircle(){

}

blMppGeneratorShapeCircleIn3D::blMppGeneratorShapeCircleIn3D(blImage* birthMap)
    : blMppGeneratorShapeCircle(birthMap){

}

blMppGeneratorShapeCircleIn3D::blMppGeneratorShapeCircleIn3D(blImageSize size)
    : blMppGeneratorShapeCircle(size){

}

blMppGeneratorShapeCircleIn3D::~blMppGeneratorShapeCircleIn3D(){

}

// blProcess methods
std::string blMppGeneratorShapeCircleIn3D::name(){
    return "blMppGeneratorShapeCircleIn3D";
}

// ----------------------- Virtual methods for Shape management ------------
blMppShape* blMppGeneratorShapeCircleIn3D::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int (blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeCircle* templateShape = m_shapeLibrary[pos];
    int r = templateShape->radius();

    // randomly get center
    int x, y,z;
    int posMin;
    posMin = r + m_borderWidth;

    int posMaxNl = m_birthArea.xSize() - posMin;
    int posMaxNc = m_birthArea.ySize() - posMin;
    int posMaxNz = m_birthArea.zSize();
    bool stop = false;

    while (!stop){
        blPixel pixelIndex = getRandomPixelFromMask();
        if (pixelIndex.x() >= posMin &&  pixelIndex.x() < posMaxNl
                && pixelIndex.y() >= posMin &&  pixelIndex.y() < posMaxNc
                && pixelIndex.z() >= 0 &&  pixelIndex.z() < posMaxNz){
            x = pixelIndex.x();
            y = pixelIndex.y();
            z = pixelIndex.z();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeCircle *newShape = new blMppShapeCircle();
    calculateShapeData(blPixel(x,y,z), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeCircleIn3D::generateRandomUsingArea(){

    int pos = int (blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeCircle* templateShape = m_shapeLibrary[pos];
    int r = templateShape->radius();

    int posMin;
    posMin = r + m_borderWidth;
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int rangeMaxNz = m_birthArea.zSize();
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + posMin);
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + posMin);
    int z = int(blRandomGeneratorPM::rand()*float(rangeMaxNz));

    //std::cout << "circle = (" << x << ", " << y << ", " << z << ", " << r << ")" << std::endl;
    blMppShapeCircle *newShape = new blMppShapeCircle();
    calculateShapeData(blPixel(x,y,z), templateShape, newShape);
    return newShape;
}

// virtual
blMppShape* blMppGeneratorShapeCircleIn3D::translate(blMppShape* shape){

    //std::cout << "do translation circle" << std::endl;
    blMppShapeCircle* shapesq = dynamic_cast<blMppShapeCircle*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;
    int z = shapesq->getCenter().z();

    // control if still inside image
    int posMin;
    int a = shapesq->radius();
    posMin = a + m_borderWidth;
    int posMaxNl = m_birthArea.xSize()-posMin;
    int posMaxNc = m_birthArea.ySize()-posMin;

    // refuse the on direction of the translation if the object goes out of the image
    if (x <= posMin){
        x = shapesq->getCenter().x();
        deltaX = 0;
    }
    if (x >= posMaxNl){
        x = shapesq->getCenter().x();
        deltaX = 0;
    }
    if (y <= posMin){
        y = shapesq->getCenter().y();
        deltaY = 0;
    }
    if (y >= posMaxNc){
        y = shapesq->getCenter().y();
        deltaY = 0;
    }

    //std::cout << "deltaX = " << deltaX << "deltaY = " << deltaY << std::endl;
    //std::cout << "init circle: x = " << shapesq->getCenter().x() << ", y = " << shapesq->getCenter().y() << ", r = " << shapesq->radius() << std::endl;
    //std::cout << "translated circle: x = " << x << ", y = " << y << ", r = " << shapesq->radius() << std::endl;

    // fill shape
    blMppShapeCircle *newShape = new blMppShapeCircle();
    // copy parameters
    newShape->setCenter(blPixel(x,y,z));
    newShape->setParameters(shapesq->radius());
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // translate silhouette
    newShape->setSilhouette(shapesq->silhouette()->translate(blPixel(deltaX, deltaY,0)));
    // translate border
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapesq->borderPixels(), blPixel(deltaX, deltaY,0), translated);
        newShape->setBorderPixels(translated);
    }
    // copy normals
    if (m_calculateNormals){
        copyNormals(shapesq, newShape);
    }

    return newShape;
}
