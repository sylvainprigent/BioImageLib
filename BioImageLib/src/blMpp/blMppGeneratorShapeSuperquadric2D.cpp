/// \file blMppGeneratorShapeSuperquadric2D.cpp
/// \brief blMppGeneratorShapeSuperquadric2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeSuperquadric2D.h"
using namespace std;

#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

blMppGeneratorShapeSuperquadric2D::blMppGeneratorShapeSuperquadric2D() : blMppGeneratorShape(){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeSuperquadric2D::blMppGeneratorShapeSuperquadric2D(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeSuperquadric2D::blMppGeneratorShapeSuperquadric2D(blImageSize size)
    : blMppGeneratorShape(size){
    initParametersWithDefaultsValues();
}

blMppGeneratorShapeSuperquadric2D::~blMppGeneratorShapeSuperquadric2D(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeSuperquadric2D::name(){
    return "blMppGeneratorShapeSuperquadric2D";
}

void blMppGeneratorShapeSuperquadric2D::initParametersWithDefaultsValues(){
    setMajorAxisRange(3, 5);
    setMinorAxisRange(2, 4);
    setMajorExposantRange(2, 6, 2);
    setMinorExposantRange(2, 6, 2);
    setThetaRange(0, 3.14, 0.2);
}

// setters parameters
void blMppGeneratorShapeSuperquadric2D::setMajorAxisRange(int aMin, int aMax){
    m_aMin = aMin;
    m_aMax = aMax;
    m_aRange = m_aMax - m_aMin + 1;

    if (m_aMin <= 0 || m_aMax <= 0 || m_aMin > m_aMax){
        throw( blException("blMppGeneratorShapeSuperquadric2D: Major axis setting is not correct"));
    }
}

void blMppGeneratorShapeSuperquadric2D::setMinorAxisRange(int bMin, int bMax){
    m_bMin = bMin; m_bMax = bMax;
    m_bRange = m_bMax - m_bMin + 1;
    if (m_bMin <= 0 || m_bMax <= 0 || m_bMin > m_bMax){
        throw( blException("blMppGeneratorShapeSuperquadric2D: Minor axis setting is not correct"));
    }
}

void blMppGeneratorShapeSuperquadric2D::setMajorExposantRange(float mMin, float mMax, float mStep){
    m_mMin = mMin;
    m_mMax = mMax;
    m_mStep = mStep;

    m_exposantXSpace = 0;
    for (float i = m_mMin ; i <= m_mMax ; i+=m_mStep ){
        m_exposantXSpace++;
    }

    if (m_exposantXSpace == 0){
        throw( blException("blMppGeneratorShapeSuperquadric2D: There should be at least one X exposant"));
    }
}

void blMppGeneratorShapeSuperquadric2D::setMinorExposantRange(float nMin, float nMax, float nStep){
    m_nMin = nMin;
    m_nMax = nMax;
    m_nStep = nStep;


    m_exposantYSpace = 0;
    for (float i = m_nMin ; i <= m_nMax ; i+=m_nStep ){
        m_exposantYSpace++;
    }

    if (m_exposantYSpace == 0){
        throw( blException("blMppGeneratorShapeSuperquadric2D: There should be at least one Y exposant"));
    }
}

void blMppGeneratorShapeSuperquadric2D::setThetaRange(float thetaMin, float thetaMax, float thetaStep){
    m_thetaMin = thetaMin;
    m_thetaMax = thetaMax;
    m_thetaStep = thetaStep;

    m_anglesNumber = 0;
    for (float i = m_thetaMin ; i <= m_thetaMax ; i+=m_thetaStep){
        m_anglesNumber++;
    }

    if (m_anglesNumber < 1){
        throw( blException("blMppGeneratorShapeSuperquadric2D: There should be at least one angle"));
    }
}

// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeSuperquadric2D::initialize(){

    int sizeLib = m_aRange*m_bRange*m_exposantXSpace*m_exposantYSpace*m_anglesNumber;
    m_shapeLibrary.resize(sizeLib);

    int iter = -1;
    int b_val;
    for (int a = m_aMin ; a <= m_aMax ; ++a){
        for (int b = m_bMin ; b <= m_bMax ; ++b){
            for (float m = m_mMin ; m <= m_mMax ; m+= m_mStep){
                for (float n = m_nMin ; n <= m_nMax ; n+= m_nStep){
                    for (float theta = m_thetaMin ; theta <= m_thetaMax ; theta+=m_thetaStep){
                        blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
                        newShape->setCenter(blPixel(0,0));
                        b_val = b;
                        if (b_val > a){
                            b_val = a;
                        }
                        newShape->setParameters(a, b_val, m, n, theta );
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
        }
    }
    //std::cout << "shape library size = " <<  m_shapeLibrary.size() << std::endl;
    this->notify("shape library size = " + m_shapeLibrary.size());
}

blMppShape* blMppGeneratorShapeSuperquadric2D::generateRandomUsingMask(){

    // randomly get a shape parameter set

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSuperquadric2D* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();

    // randomly get center
    int x, y;
    int posMin;
    if (templateShape->m() > 2 || templateShape->n() > 2){
        posMin = int(sqrt(a*a + b*b))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
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
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    calculateShapeData(blPixel(x,y), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeSuperquadric2D::generateRandomUsingArea(){


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
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + float(posMin));
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + float(posMin));

    //std::cout << "normal before = " << templateShape->normals()->operator [](0)[0] << ", " << templateShape->normals()->operator [](0)[1] << std::endl;
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    calculateShapeData(blPixel(x,y), templateShape, newShape);
    return newShape;
}

void blMppGeneratorShapeSuperquadric2D::calculateShapeData(blPixel centerPixel, blMppShapeSuperquadric2D* templateShape, blMppShapeSuperquadric2D *newShape){

    // parameters
    newShape->setCenter(centerPixel);
    newShape->setParameters(templateShape->a(), templateShape->b(), templateShape->m(), templateShape->n(), templateShape->theta() );
    newShape->computeMaxDistanceToBarycenter();
    //newShape->setMaxDistanceToBarycenter(templateShape->maxDistanceToBarycenter());

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

void blMppGeneratorShapeSuperquadric2D::copyNormals(blMppShapeSuperquadric2D* templateShape, blMppShapeSuperquadric2D *newShape){
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

void blMppGeneratorShapeSuperquadric2D::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y(), (*itPixels).z() + t.z());
    }
}

int blMppGeneratorShapeSuperquadric2D::shapesMaxLength(){
    return 2.0*sqrt(m_aMax*m_aMax + m_bMax*m_bMax);
}

// virtual
blMppShape* blMppGeneratorShapeSuperquadric2D::translate(blMppShape* shape){

    //std::cout << "do translation" << std::endl;
    blMppShapeSuperquadric2D* shapesq = dynamic_cast<blMppShapeSuperquadric2D*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;


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
    newShape->setCenter(blPixel(x,y));
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->m(), shapesq->n(), shapesq->theta() );
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

blMppShape* blMppGeneratorShapeSuperquadric2D::rotate(blMppShape* shape){

    //std::cout << "do rotate" << std::endl;
    // get a perturbated angle
    blMppShapeSuperquadric2D* shapesq = dynamic_cast<blMppShapeSuperquadric2D*>(shape);

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
    int posm = int((shapesq->m() - m_mMin)/m_mStep);
    int posn = int((shapesq->n() - m_nMin)/m_nStep);
    int posr = int((angle - m_thetaMin)/m_thetaStep);

    int id1 = m_anglesNumber*posn+posr;
    int id2 = m_anglesNumber*m_exposantYSpace*posm+id1;
    int id3 = m_anglesNumber*m_exposantYSpace*m_exposantYSpace*posb + id2;
    int idx = m_anglesNumber*m_exposantYSpace*m_exposantYSpace*m_bRange*posa+id3;

    // create the new shape
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->m(), shapesq->n(), angle );
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // set silhouette
    blMppShapeSuperquadric2D *shapeTemplate = m_shapeLibrary[idx];
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

blMppShape* blMppGeneratorShapeSuperquadric2D::resize(blMppShape* shape){

    //std::cout << "do resize" << std::endl;
    blMppShapeSuperquadric2D* shapesq = dynamic_cast<blMppShapeSuperquadric2D*>(shape);

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

    //std::cout << "resize (" <<  shapesq->getCenter().x() << ", " << shapesq->getCenter().y()
    //          <<", " << shapesq->a() << ", " << shapesq->b() << std::endl;
    //std::cout << "to (" <<  shapesq->getCenter().x() << ", " << shapesq->getCenter().y()
    //          <<", " << a << ", " << b << std::endl;

    // find the appropriate template
    int posa = a - m_aMin;
    int posb = b - m_bMin;
    int posm = int((shapesq->m() - m_mMin)/m_mStep);
    int posn = int((shapesq->n() - m_nMin)/m_nStep);
    int posr = int((shapesq->theta() - m_thetaMin)/m_thetaStep);

    int id1 = m_anglesNumber*posn+posr;
    int id2 = m_anglesNumber*m_exposantYSpace*posm+id1;
    int id3 = m_anglesNumber*m_exposantYSpace*m_exposantYSpace*posb + id2;
    int idx = m_anglesNumber*m_exposantYSpace*m_exposantYSpace*m_bRange*posa+id3;

    // create the new shape
    blMppShapeSuperquadric2D *newShape = new blMppShapeSuperquadric2D();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(a, b, shapesq->m(), shapesq->n(), shapesq->theta() );
    newShape->computeMaxDistanceToBarycenter();

    // set silhouette
    blMppShapeSuperquadric2D *shapeTemplate = m_shapeLibrary[idx];
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
