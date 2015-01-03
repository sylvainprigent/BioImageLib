/// \file blMppGeneratorShapeSuperquadric2DIn3D.cpp
/// \brief blMppGeneratorShapeSuperquadric2DIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeSuperquadric2DIn3D.h"
using namespace std;

#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

blMppGeneratorShapeSuperquadric2DIn3D::blMppGeneratorShapeSuperquadric2DIn3D() : blMppGeneratorShapeSuperquadric2D(){

}

blMppGeneratorShapeSuperquadric2DIn3D::blMppGeneratorShapeSuperquadric2DIn3D(blImage* birthMap)
    : blMppGeneratorShapeSuperquadric2D(birthMap){
}

blMppGeneratorShapeSuperquadric2DIn3D::blMppGeneratorShapeSuperquadric2DIn3D(blImageSize size)
    : blMppGeneratorShapeSuperquadric2D(size){
}

blMppGeneratorShapeSuperquadric2DIn3D::~blMppGeneratorShapeSuperquadric2DIn3D(){

}

// blProcess methods
std::string blMppGeneratorShapeSuperquadric2DIn3D::name(){
    return "blMppGeneratorShapeSuperquadric2DIn3D";
}


// ----------------------- Virtual methods for Shape management ------------
blMppShape* blMppGeneratorShapeSuperquadric2DIn3D::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSuperquadric2D* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();

    // randomly get center
    int x, y, z;
    int posMin;
    if (templateShape->m() > 2 || templateShape->n() > 2){
        posMin = int(sqrt(a*a + b*b))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
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
                && pixelIndex.z() >= 0 &&  pixelIndex.z() < posMaxNz){
            x = pixelIndex.x();
            y = pixelIndex.y();
            z = pixelIndex.z();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    calculateShapeData(blPixel(x,y,z), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeSuperquadric2DIn3D::generateRandomUsingArea(){

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSuperquadric2D* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();

    int posMin;
    if (templateShape->m() > 2 || templateShape->n() > 2){
        posMin = int(sqrt(a*a + b*b))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int posMaxNz = m_birthArea.zSize();
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + float(posMin));
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + float(posMin));
    int z = int(blRandomGeneratorPM::rand()*float(posMaxNz));

    //std::cout << "normal before = " << templateShape->normals()->operator [](0)[0] << ", " << templateShape->normals()->operator [](0)[1] << std::endl;
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    calculateShapeData(blPixel(x,y,z), templateShape, newShape);
    return newShape;
}

// virtual
blMppShape* blMppGeneratorShapeSuperquadric2DIn3D::translate(blMppShape* shape){

    //std::cout << "do translation" << std::endl;
    blMppShapeSuperquadric2D* shapesq = dynamic_cast<blMppShapeSuperquadric2D*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;
    int z = shapesq->getCenter().z();


    // control if still inside image
    int posMin;
    int a = shapesq->a();
    int b = shapesq->b();
    if (shapesq->m() > 2 || shapesq->n() > 2){
        posMin = int(sqrt(a*a + b*b))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
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
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    // copy parameters
    newShape->setCenter(blPixel(x,y,z));
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->m(), shapesq->n(), shapesq->theta() );
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
