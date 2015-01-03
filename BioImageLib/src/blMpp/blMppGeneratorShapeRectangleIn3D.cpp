/// \file blMppGeneratorShapeRectangleIn3D.cpp
/// \brief blMppGeneratorShapeRectangleIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeRectangleIn3D.h"
using namespace std;

#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

blMppGeneratorShapeRectangleIn3D::blMppGeneratorShapeRectangleIn3D() : blMppGeneratorShapeRectangle(){

}

blMppGeneratorShapeRectangleIn3D::blMppGeneratorShapeRectangleIn3D(blImage* birthMap)
    : blMppGeneratorShapeRectangle(birthMap){

}

blMppGeneratorShapeRectangleIn3D::blMppGeneratorShapeRectangleIn3D(blImageSize size)
    : blMppGeneratorShapeRectangle(size){

}

blMppGeneratorShapeRectangleIn3D::~blMppGeneratorShapeRectangleIn3D(){

}

// blProcess methods
std::string blMppGeneratorShapeRectangleIn3D::name(){
    return "blMppGeneratorShapeRectangleIn3D";
}

// ----------------------- Virtual methods for Shape management ------------
blMppShape* blMppGeneratorShapeRectangleIn3D::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeRectangle* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();

    // randomly get center
    int x, y, z;
    int posMin;
    posMin = int(sqrt(a*a + b*b))+ m_borderWidth;

    int posMaxNl = m_birthArea.xSize() - 2*posMin;
    int posMaxNc = m_birthArea.ySize() - 2*posMin;
    int posMaxNz = m_birthArea.zSize();
    bool stop = false;

    posMaxNl += posMin;
    posMaxNc += posMin;
    while (!stop){
        blPixel pixelIndex = getRandomPixelFromMask();
        if (pixelIndex.x() >= posMin &&  pixelIndex.x() < posMaxNl
                && pixelIndex.y() >= posMin &&  pixelIndex.y() < posMaxNc
                && pixelIndex.z() >= 0 &&  pixelIndex.z() < posMaxNz ){
            x = pixelIndex.x();
            y = pixelIndex.y();
            z = pixelIndex.z();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeRectangle *newShape = new blMppShapeRectangle();
    calculateShapeData(blPixel(x,y,z), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeRectangleIn3D::generateRandomUsingArea(){

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeRectangle* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();

    int posMin;
    posMin = int(sqrt(a*a + b*b))+ m_borderWidth;
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int rangeMaxNz = m_birthArea.zSize();
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + posMin);
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + posMin);
    int z = int(blRandomGeneratorPM::rand()*float(rangeMaxNz));

    //std::cout << "normal before = " << templateShape->normals()->operator [](0)[0] << ", " << templateShape->normals()->operator [](0)[1] << std::endl;
    blMppShapeRectangle *newShape = new blMppShapeRectangle();
    calculateShapeData(blPixel(x,y,z), templateShape, newShape);
    return newShape;
}

// virtual
blMppShape* blMppGeneratorShapeRectangleIn3D::translate(blMppShape* shape){

    //std::cout << "do translation" << std::endl;
    blMppShapeRectangle* shapesq = dynamic_cast<blMppShapeRectangle*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;
    int z = shapesq->getCenter().z();

    // control if still inside image
    int posMin;
    int a = shapesq->a();
    int b = shapesq->b();
    posMin = int(sqrt(a*a + b*b))+ m_borderWidth;
    int posMaxNl = m_birthArea.xSize() - posMin;
    int posMaxNc = m_birthArea.ySize() - posMin;

    // refuse one direction of the translation if the object goes out of the image
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

    // fill shape
    blMppShapeRectangle *newShape = new blMppShapeRectangle();
    // copy parameters
    newShape->setCenter(blPixel(x,y,z));
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->theta() );
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // translate silhouette
    newShape->setSilhouette(shapesq->silhouette()->translate(blPixel(deltaX, deltaY, 0)));
    // translate border
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapesq->borderPixels(), blPixel(deltaX, deltaY, 0), translated);
        newShape->setBorderPixels(translated);
    }
    // copy normals
    if (m_calculateNormals){
        copyNormals(shapesq, newShape);
    }

    return newShape;
}
