/// \file blMppGeneratorShapeEllipseIn3D.cpp
/// \brief blMppGeneratorShapeEllipseIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeEllipseIn3D.h"
using namespace std;

#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

blMppGeneratorShapeEllipseIn3D::blMppGeneratorShapeEllipseIn3D() : blMppGeneratorShapeEllipse(){

}

blMppGeneratorShapeEllipseIn3D::blMppGeneratorShapeEllipseIn3D(blImage* birthMap)
    : blMppGeneratorShapeEllipse(birthMap){

}

blMppGeneratorShapeEllipseIn3D::blMppGeneratorShapeEllipseIn3D(blImageSize size)
    : blMppGeneratorShapeEllipse(size){

}

blMppGeneratorShapeEllipseIn3D::~blMppGeneratorShapeEllipseIn3D(){

}

// blProcess methods
std::string blMppGeneratorShapeEllipseIn3D::name(){
    return "blMppGeneratorShapeEllipseIn3D";
}

// ----------------------- Virtual methods for Shape management ------------
blMppShape* blMppGeneratorShapeEllipseIn3D::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeEllipse* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();

    // randomly get center
    int x, y, z;
    int posMin;
    posMin = a + m_borderWidth;

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
    blMppShapeEllipse *newShape = new blMppShapeEllipse();
    calculateShapeData(blPixel(x,y,z), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeEllipseIn3D::generateRandomUsingArea(){

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeEllipse* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();

    int posMin;
    posMin = a + m_borderWidth;
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int rangeMaxNz = m_birthArea.zSize();
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + posMin);
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + posMin);
    int z = int(blRandomGeneratorPM::rand()*float(rangeMaxNz));

    blMppShapeEllipse *newShape = new blMppShapeEllipse();
    calculateShapeData(blPixel(x,y,z), templateShape, newShape);

    return newShape;
}

// virtual
blMppShape* blMppGeneratorShapeEllipseIn3D::translate(blMppShape* shape){

    //std::cout << "do translation" << std::endl;
    blMppShapeEllipse* shapesq = dynamic_cast<blMppShapeEllipse*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;
    int z = shapesq->getCenter().z();


    // control if still inside image
    int posMin;
    int a = shapesq->a();
    posMin = a + m_borderWidth;

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
    blMppShapeEllipse *newShape = new blMppShapeEllipse();
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
