/// \file blMppGeneratorShapeCircle.cpp
/// \brief blMppGeneratorShapeCircle class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeCircle.h"
using namespace std;

#include <blMath>
#include <blRand/blRandomGeneratorPM.h>

blMppGeneratorShapeCircle::blMppGeneratorShapeCircle() : blMppGeneratorShape(){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeCircle::blMppGeneratorShapeCircle(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeCircle::blMppGeneratorShapeCircle(blImageSize size)
    : blMppGeneratorShape(size){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeCircle::~blMppGeneratorShapeCircle(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeCircle::name(){
    return "blMppGeneratorShapeCircle";
}

void blMppGeneratorShapeCircle::initParametersWithDefaultsValues(){
    setRadiusRange(2, 3);
}

// setters parameters
void blMppGeneratorShapeCircle::setRadiusRange(int minR, int maxR){
    m_rMin = minR;
    m_rMax = maxR;
    m_rRange = m_rMax - m_rMin + 1;


    if (m_rMin <= 0 || m_rMax <= 0){
        throw( blException("blMppGeneratorShapeCircle: Radius setting is not correct"));
    }
}

// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeCircle::initialize(){

    int sizeLib = m_rRange;
    m_shapeLibrary.resize(sizeLib);

    int iter = -1;
    for (int r = m_rMin ; r <= m_rMax ; ++r){
        blMppShapeCircle *newShape = new blMppShapeCircle();
        newShape->setCenter(blPixel(0,0));
        newShape->setParameters(r);
        newShape->computeSilhouette();
        newShape->computeMaxDistanceToBarycenter();
        if (m_calculateBorderPixels){
            //std::cout << "calculate border pixels" << std::endl;
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

blMppShape* blMppGeneratorShapeCircle::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int (blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeCircle* templateShape = m_shapeLibrary[pos];
    int r = templateShape->radius();

    // randomly get center
    int x, y;
    int posMin;
    posMin = r + m_borderWidth;

    int posMaxNl = m_birthArea.xSize() - 2*posMin;
    int posMaxNc = m_birthArea.ySize() - 2*posMin;
    bool stop = false;

    posMaxNl += posMin;
    posMaxNc += posMin;
    while (!stop){
        blPixel pixelIndex = getRandomPixelFromMask();
        if (pixelIndex.x() >= posMin &&  pixelIndex.x() < posMaxNl && pixelIndex.y() >= posMin &&  pixelIndex.y() < posMaxNc){
            x = pixelIndex.x();
            y = pixelIndex.y();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeCircle *newShape = new blMppShapeCircle();
    calculateShapeData(blPixel(x,y), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeCircle::generateRandomUsingArea(){

    int pos = int (blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeCircle* templateShape = m_shapeLibrary[pos];
    int r = templateShape->radius();

    int posMin;
    posMin = r + m_borderWidth;
    //std::cout << "pos min = " << posMin << std::endl;
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + posMin);
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + posMin);

    //std::cout << "circle = (" << x << ", " << y << ", " << r << ")" << std::endl;
    blMppShapeCircle *newShape = new blMppShapeCircle();
    calculateShapeData(blPixel(x,y), templateShape, newShape);
    return newShape;
}

void blMppGeneratorShapeCircle::calculateShapeData(blPixel centerPixel, blMppShapeCircle* templateShape, blMppShapeCircle *newShape){

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

void blMppGeneratorShapeCircle::copyNormals(blMppShapeCircle* templateShape, blMppShapeCircle *newShape){
    // copy normals
    std::vector<std::vector<float> >* template_normals = templateShape->normalsBorder();
    std::vector<std::vector<float> >* new_normals = new std::vector<std::vector<float> >();
    new_normals->resize(template_normals->size());
    for (int i = 0 ; i < template_normals->size() ; ++i){
        std::vector<float> inter; inter.resize(2);
        inter[0] = template_normals->operator [](i)[0];
        inter[1] = template_normals->operator [](i)[1];
        new_normals->operator [](i) = inter;
    }
    newShape->setNormalsBorder(new_normals);
}

void blMppGeneratorShapeCircle::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    //std::cout << "translate factor = " << t.x() << ", " << t.y() << std::endl;
    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y(), (*itPixels).z() + t.z());
    }
}

int blMppGeneratorShapeCircle::shapesMaxLength(){
    return 2.0*m_rMax;
}

// virtual
blMppShape* blMppGeneratorShapeCircle::translate(blMppShape* shape){

    //std::cout << "do translation circle" << std::endl;
    blMppShapeCircle* shapesq = dynamic_cast<blMppShapeCircle*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;

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
    newShape->setCenter(blPixel(x,y));
    newShape->setParameters(shapesq->radius());
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // translate silhouette
    newShape->setSilhouette(shapesq->silhouette()->translate(blPixel(deltaX, deltaY)));
    // translate border
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapesq->borderPixels(), blPixel(deltaX, deltaY), translated);
        newShape->setBorderPixels(translated);
    }
    // copy normals
    if (m_calculateNormals){
        copyNormals(shapesq, newShape);
    }

    return newShape;
}

blMppShape* blMppGeneratorShapeCircle::rotate(blMppShape* shape){

    //std::cout << "do rotate circle" << std::endl;
    // get a perturbated angle
    blMppShapeCircle* shapesq = dynamic_cast<blMppShapeCircle*>(shape);

    // find the appropriate template
    int idx = shapesq->radius() - m_rMin;

    // create the new shape
    blMppShapeCircle *newShape = new blMppShapeCircle();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(shapesq->radius());
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // set silhouette
    blMppShapeCircle *shapeTemplate = m_shapeLibrary[idx];
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

blMppShape* blMppGeneratorShapeCircle::resize(blMppShape* shape){

    //std::cout << "do resize circle" << std::endl;
    blMppShapeCircle* shapesq = dynamic_cast<blMppShapeCircle*>(shape);

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

    // find the appropriate template
    int idx = a - m_rMin;

    // create the new shape
    blMppShapeCircle *newShape = new blMppShapeCircle();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(a);
    newShape->computeMaxDistanceToBarycenter();

    // set silhouette
    blMppShapeCircle *shapeTemplate = m_shapeLibrary[idx];
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

