/// \file blMppGeneratorShapeEllipse.cpp
/// \brief blMppGeneratorShapeEllipse class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeEllipse.h"
using namespace std;

#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

blMppGeneratorShapeEllipse::blMppGeneratorShapeEllipse() : blMppGeneratorShape(){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeEllipse::blMppGeneratorShapeEllipse(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeEllipse::blMppGeneratorShapeEllipse(blImageSize size)
    : blMppGeneratorShape(size){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeEllipse::~blMppGeneratorShapeEllipse(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeEllipse::name(){
    return "blMppGeneratorShapeEllipse";
}

void blMppGeneratorShapeEllipse::initParametersWithDefaultsValues(){
    setMajorAxisRange(3, 5);
    setMinorAxisRange(2, 4);
    setThetaRange(0, 3.14, 0.2);
}

// setters parameters
void blMppGeneratorShapeEllipse::setMajorAxisRange(int aMin, int aMax){
    m_aMin = aMin;
    m_aMax = aMax;
    m_aRange = m_aMax - m_aMin + 1;

    //std::cout << "set axis ellipse setMajorAxisRange = " << m_aMin << ", " << m_aMax << std::endl;

    if (m_aMin <= 0 || m_aMax <= 0 || m_aMin > m_aMax){
        throw( blException("blMppGeneratorShapeEllipse: Major axis setting is not correct"));
    }
}

void blMppGeneratorShapeEllipse::setMinorAxisRange(int bMin, int bMax){
    m_bMin = bMin; m_bMax = bMax;
    m_bRange = m_bMax - m_bMin + 1;
    if (m_bMin <= 0 || m_bMax <= 0 || m_bMin > m_bMax){
        throw( blException("blMppGeneratorShapeEllipse: Minor axis setting is not correct"));
    }
}

void blMppGeneratorShapeEllipse::setThetaRange(float thetaMin, float thetaMax, float thetaStep){
    m_thetaMin = thetaMin;
    m_thetaMax = thetaMax;
    m_thetaStep = thetaStep;

    m_anglesNumber = 0;
    for (float i = m_thetaMin ; i <= m_thetaMax ; i+=m_thetaStep){
        m_anglesNumber++;
    }

    if (m_anglesNumber < 1){
        throw( blException("blMppGeneratorShapeEllipse: There should be at least one angle"));
    }
}

// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeEllipse::initialize(){

    int sizeLib = m_aRange*m_bRange*m_anglesNumber;
    m_shapeLibrary.resize(sizeLib);

    int iter = -1;
    int b_val;
    for (int a = m_aMin ; a <= m_aMax ; ++a){
        for (int b = m_bMin ; b <= m_bMax ; ++b){
            for (float theta = m_thetaMin ; theta <= m_thetaMax ; theta+=m_thetaStep){
                blMppShapeEllipse *newShape = new blMppShapeEllipse();
                newShape->setCenter(blPixel(0,0));
                b_val = b;
                if (b_val > a){
                    b_val = a;
                }
                newShape->setParameters(a, b_val, theta );
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
        }
    }
    //std::cout << "shape library size = " <<  m_shapeLibrary.size() << std::endl;
    this->notify("shape library size = " + m_shapeLibrary.size());
}

blMppShape* blMppGeneratorShapeEllipse::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeEllipse* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();

    // randomly get center
    int x, y;
    int posMin;
    posMin = a + m_borderWidth;

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
    blMppShapeEllipse *newShape = new blMppShapeEllipse();
    calculateShapeData(blPixel(x,y), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeEllipse::generateRandomUsingArea(){

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeEllipse* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();

    int posMin;
    posMin = a + m_borderWidth;
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + posMin);
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + posMin);

    blMppShapeEllipse *newShape = new blMppShapeEllipse();
    calculateShapeData(blPixel(x,y), templateShape, newShape);
    return newShape;
}

void blMppGeneratorShapeEllipse::calculateShapeData(blPixel centerPixel, blMppShapeEllipse* templateShape, blMppShapeEllipse *newShape){

    // parameters
    newShape->setCenter(centerPixel);
    newShape->setParameters(templateShape->a(), templateShape->b(), templateShape->theta() );
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

void blMppGeneratorShapeEllipse::copyNormals(blMppShapeEllipse* templateShape, blMppShapeEllipse *newShape){
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

void blMppGeneratorShapeEllipse::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y(), (*itPixels).z() + t.z());
    }
}

int blMppGeneratorShapeEllipse::shapesMaxLength(){
    return 2.0*m_aMax;
}

// virtual
blMppShape* blMppGeneratorShapeEllipse::translate(blMppShape* shape){

    //std::cout << "do translation" << std::endl;
    blMppShapeEllipse* shapesq = dynamic_cast<blMppShapeEllipse*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;


    // control if still inside image
    int posMin;
    int a = shapesq->a();
    int b = shapesq->b();
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
    newShape->setCenter(blPixel(x,y));
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->theta() );
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

blMppShape* blMppGeneratorShapeEllipse::rotate(blMppShape* shape){

    //std::cout << "do rotate" << std::endl;
    // get a perturbated angle
    blMppShapeEllipse* shapesq = dynamic_cast<blMppShapeEllipse*>(shape);

    float angle = shapesq->theta() + blMath::randSign()*m_deltaRotation;
    if (angle < m_thetaMin){
        angle = m_thetaMin;
    }
    if (angle > m_thetaMax){
        angle = m_thetaMax;
    }

    // find the appropriate template
    int posa = shapesq->a() - m_aMin;
    int posb = shapesq->b() - m_bMin;
    int posr = int((angle - m_thetaMin)/m_thetaStep);

    int id1 = m_anglesNumber*posb+posr;
    int idx = m_anglesNumber*m_bRange*posa+id1;

    // create the new shape
    blMppShapeEllipse *newShape = new blMppShapeEllipse();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(shapesq->a(), shapesq->b(), angle );
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // set silhouette
    blMppShapeEllipse *shapeTemplate = m_shapeLibrary[idx];
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

blMppShape* blMppGeneratorShapeEllipse::resize(blMppShape* shape){

    //std::cout << "do resize" << std::endl;
    blMppShapeEllipse* shapesq = dynamic_cast<blMppShapeEllipse*>(shape);

    // get a perturbated angle
    int a = shapesq->a() + blMath::randSign()*m_deltaResize;
    int b = shapesq->b() + blMath::randSign()*m_deltaResize;
    if (a > m_aMax){ a = m_aMax;}
    if (a < m_aMin){ a = m_aMin;}
    if (b > m_bMax){ b = m_bMax;}
    if (b < m_bMin){ b = m_bMin;}

    // refuse resize if the object goes out of the image
    if ( shapesq->getCenter().x() + a + m_borderWidth >= m_birthArea.xSize()
         || shapesq->getCenter().x() - a - m_borderWidth <= 0
         || shapesq->getCenter().y() + a + m_borderWidth >= m_birthArea.ySize()
         || shapesq->getCenter().y() - a - m_borderWidth <= 0
         ){
        a = shapesq->a();
        if (b > a){
            b = a;
        }
    }

    // find the appropriate template
    int posa = a - m_aMin;
    int posb = b - m_bMin;
    int posr = int((shapesq->theta() - m_thetaMin)/m_thetaStep);

    int id1 = m_anglesNumber*posb+posr;
    int idx = m_anglesNumber*m_bRange*posa+id1;

    // create the new shape
    blMppShapeEllipse *newShape = new blMppShapeEllipse();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(a, b, shapesq->theta() );
    newShape->computeMaxDistanceToBarycenter();

    // set silhouette
    blMppShapeEllipse *shapeTemplate = m_shapeLibrary[idx];
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

