/// \file blMppGeneratorShapeSphere.cpp
/// \brief blMppGeneratorShapeSphere class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeSphere.h"
using namespace std;

#include <blMath>
#include <blRand/blRandomGeneratorPM.h>

blMppGeneratorShapeSphere::blMppGeneratorShapeSphere() : blMppGeneratorShape(){
    initParametersWithDefaultsValues();
    m_is3DShapes = true;
}

blMppGeneratorShapeSphere::blMppGeneratorShapeSphere(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initParametersWithDefaultsValues();
    m_is3DShapes = true;
}

blMppGeneratorShapeSphere::blMppGeneratorShapeSphere(blImageSize size)
    : blMppGeneratorShape(size){
    initParametersWithDefaultsValues();
    m_is3DShapes = true;
}

blMppGeneratorShapeSphere::~blMppGeneratorShapeSphere(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeSphere::name(){
    return "blMppGeneratorShapeSphere";
}

void blMppGeneratorShapeSphere::initParametersWithDefaultsValues(){
    setRadiusRange(2, 3);
}

// setters parameters
void blMppGeneratorShapeSphere::setRadiusRange(int minR, int maxR){
    m_rMin = minR;
    m_rMax = maxR;
    m_rRange = m_rMax - m_rMin + 1;


    if (m_rMin <= 0 || m_rMax <= 0){
        throw( blException("blMppGeneratorShapeSphere: Radius setting is not correct"));
    }
}

// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeSphere::initialize(){

    int sizeLib = m_rRange;
    m_shapeLibrary.resize(sizeLib);

    int iter = -1;
    for (int r = m_rMin ; r <= m_rMax ; ++r){
        blMppShapeSphere *newShape = new blMppShapeSphere();
        newShape->setCenter(blPixel(0,0,0));
        newShape->setParameters(r);
        newShape->computeSilhouette();
        //std::cout << "new silhouette : " << std::endl; newShape->silhouette()->printConsole(); std::cout << std::endl;
        newShape->computeMaxDistanceToBarycenter();
        if (m_calculateBorderPixels){
            newShape->computeBorderPixels(m_borderWidth);
        }
        if (m_calculateNormals){
            newShape->computeNormalsBorder();
        }
        iter++;
        m_shapeLibrary[iter] = newShape;
    }

    this->notify("shape library size = " + m_shapeLibrary.size());
}

blMppShape* blMppGeneratorShapeSphere::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int (blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));

    blMppShapeSphere* templateShape = m_shapeLibrary[pos];
    int r = templateShape->radius();

    // randomly get center
    int x, y, z;
    int posMin;
    posMin = r + m_borderWidth;

    int posMaxNl = m_birthArea.xSize() - posMin;
    int posMaxNc = m_birthArea.ySize() - posMin;
    int posMaxNz = m_birthArea.zSize() - posMin;
    bool stop = false;

    while (!stop){
        blPixel pixelIndex = getRandomPixelFromMask();
        if (pixelIndex.x() >= posMin &&  pixelIndex.x() < posMaxNl
                && pixelIndex.y() >= posMin &&  pixelIndex.y() < posMaxNc
                && pixelIndex.z() >= posMin &&  pixelIndex.z() < posMaxNz
            ){
            x = pixelIndex.x();
            y = pixelIndex.y();
            z = pixelIndex.z();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeSphere *newShape = new blMppShapeSphere();
    calculateShapeData(blPixel(x,y,z), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeSphere::generateRandomUsingArea(){

    int pos = int (blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSphere* templateShape = m_shapeLibrary[pos];
    int r = templateShape->radius();

    int posMin;
    posMin = r + m_borderWidth;
    //std::cout << "pos min = " << posMin << std::endl;
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int rangeMaxNz = m_birthArea.zSize() - 2*posMin;
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + posMin);
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + posMin);
    int z = int(blRandomGeneratorPM::rand()*float(rangeMaxNz) + posMin);

    //std::cout << "circle = (" << x << ", " << y << ", " << r << ")" << std::endl;
    blMppShapeSphere *newShape = new blMppShapeSphere();
    calculateShapeData(blPixel(x,y,z), templateShape, newShape);
    return newShape;
}

void blMppGeneratorShapeSphere::calculateShapeData(blPixel centerPixel, blMppShapeSphere* templateShape, blMppShapeSphere *newShape){

    // parameters
    newShape->setCenter(centerPixel);
    newShape->setParameters(templateShape->radius());
    newShape->computeMaxDistanceToBarycenter();

    // silhouette
    newShape->setSilhouette(templateShape->silhouette()->translate(centerPixel));

    // border
    if (m_calculateBorderPixels){
        std::vector<blPixel>* translated = new std::vector<blPixel>();
        translatePixels(templateShape->borderPixels(), centerPixel,  translated);
        newShape->setBorderPixels(translated);
    }

    // normals
    if (m_calculateNormals){
        copyNormals(templateShape, newShape);
    }
}

void blMppGeneratorShapeSphere::copyNormals(blMppShapeSphere *templateShape, blMppShapeSphere *newShape){
    // copy normals
    std::vector<std::vector<float> >* template_normals = templateShape->normalsBorder();
    std::vector<std::vector<float> >* new_normals = new std::vector<std::vector<float> >();
    new_normals->resize(template_normals->size());
    for (int i = 0 ; i < template_normals->size() ; ++i){
        std::vector<float> inter; inter.resize(3);
        inter[0] = template_normals->operator [](i)[0];
        inter[1] = template_normals->operator [](i)[1];
        inter[2] = template_normals->operator [](i)[2];
        new_normals->operator [](i) = inter;
    }
    newShape->setNormalsBorder(new_normals);
}

void blMppGeneratorShapeSphere::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    //std::cout << "translate factor = " << t.x() << ", " << t.y() << std::endl;
    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y(), (*itPixels).z() + t.z());
    }
}

int blMppGeneratorShapeSphere::shapesMaxLength(){
    return 2.0*m_rMax;
}

// virtual
blMppShape* blMppGeneratorShapeSphere::translate(blMppShape* shape){

    //std::cout << "do translation sphere" << std::endl;
    blMppShapeSphere* shapesq = dynamic_cast<blMppShapeSphere*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int deltaZ = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;
    int z = shapesq->getCenter().z() + deltaZ;

    // control if still inside image
    int posMin;
    int a = shapesq->radius();
    posMin = a + m_borderWidth;
    int posMaxNl = m_birthArea.xSize()-posMin;
    int posMaxNc = m_birthArea.ySize()-posMin;
    int posMaxNz = m_birthArea.zSize()-posMin;

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
    if (z <= posMin){
        z = shapesq->getCenter().z();
        deltaZ = 0;
    }
    if (z >= posMaxNz){
        z = shapesq->getCenter().z();
        deltaZ = 0;
    }

    //std::cout << "deltaX = " << deltaX << "deltaY = " << deltaY << std::endl;
    //std::cout << "init circle: x = " << shapesq->getCenter().x() << ", y = " << shapesq->getCenter().y() << ", r = " << shapesq->radius() << std::endl;
    //std::cout << "translated circle: x = " << x << ", y = " << y << ", r = " << shapesq->radius() << std::endl;

    // fill shape
    blMppShapeSphere *newShape = new blMppShapeSphere();
    // copy parameters
    newShape->setCenter(blPixel(x,y,z));
    newShape->setParameters(shapesq->radius());
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // translate silhouette
    newShape->setSilhouette(shapesq->silhouette()->translate(blPixel(deltaX, deltaY, deltaZ)));
    // translate border
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapesq->borderPixels(), blPixel(deltaX, deltaY, deltaZ), translated);
        newShape->setBorderPixels(translated);
    }
    // copy normals
    if (m_calculateNormals){
        copyNormals(shapesq, newShape);
    }

    return newShape;
}

blMppShape* blMppGeneratorShapeSphere::rotate(blMppShape* shape){

    //std::cout << "do rotate Sphere" << std::endl;
    // get a perturbated angle
    blMppShapeSphere* shapesq = dynamic_cast<blMppShapeSphere*>(shape);

    // find the appropriate template
    int idx = shapesq->radius() - m_rMin;

    // create the new shape
    blMppShapeSphere *newShape = new blMppShapeSphere();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(shapesq->radius());
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // set silhouette
    blMppShapeSphere *shapeTemplate = m_shapeLibrary[idx];
    newShape->setSilhouette(shapeTemplate->silhouette()->translate(shapesq->getCenter()));

    // translate normals
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapeTemplate->borderPixels(), shapesq->getCenter(), translated);
        newShape->setBorderPixels(translated);
    }

    // copy normals
    if (m_calculateNormals){
        copyNormals(shapeTemplate, newShape);
    }
    return newShape;
}

blMppShape* blMppGeneratorShapeSphere::resize(blMppShape* shape){

    //std::cout << "do resize sphere" << std::endl;
    blMppShapeSphere* shapesq = dynamic_cast<blMppShapeSphere*>(shape);

    // get a perturbated angle
    int a = shapesq->radius() + blMath::randSign()*m_deltaResize;
    if (a > m_rMax){ a = m_rMax;}
    if (a < m_rMin){ a = m_rMin;}

    if (shapesq->getCenter().x() +a >= m_birthArea.xSize() - m_borderWidth
            || shapesq->getCenter().x() -a -m_borderWidth <= 0){
        a = shapesq->radius();
    }
    if (shapesq->getCenter().y() +a >= m_birthArea.ySize() - m_borderWidth
            || shapesq->getCenter().y() -a -m_borderWidth <= 0){
        a = shapesq->radius();
    }
    if (shapesq->getCenter().z() +a >= m_birthArea.zSize() - m_borderWidth
            || shapesq->getCenter().z() -a -m_borderWidth <= 0){
        a = shapesq->radius();
    }

    // find the appropriate template
    int idx = a - m_rMin;

    // create the new shape
    blMppShapeSphere *newShape = new blMppShapeSphere();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(a);
    newShape->computeMaxDistanceToBarycenter();

    // set silhouette
    blMppShapeSphere *shapeTemplate = m_shapeLibrary[idx];
    newShape->setSilhouette(shapeTemplate->silhouette()->translate(shapesq->getCenter()));

    // translate border
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapeTemplate->borderPixels(), shapesq->getCenter(), translated);
        newShape->setBorderPixels(translated);
    }

    // copy normals
    if (m_calculateNormals){
        copyNormals(shapeTemplate, newShape);
    }
    return newShape;
}

