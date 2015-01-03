/// \file blMppGeneratorShapeSuperquadric3D.cpp
/// \brief blMppGeneratorShapeSuperquadric3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeSuperquadric3D.h"
using namespace std;

#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

blMppGeneratorShapeSuperquadric3D::blMppGeneratorShapeSuperquadric3D() : blMppGeneratorShape(){
    initParametersWithDefaultsValues();
    m_is3DShapes = true;
}

blMppGeneratorShapeSuperquadric3D::blMppGeneratorShapeSuperquadric3D(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initParametersWithDefaultsValues();
    m_is3DShapes = true;
}

blMppGeneratorShapeSuperquadric3D::blMppGeneratorShapeSuperquadric3D(blImageSize size)
    : blMppGeneratorShape(size){
    initParametersWithDefaultsValues();
    m_is3DShapes = true;
}

blMppGeneratorShapeSuperquadric3D::~blMppGeneratorShapeSuperquadric3D(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeSuperquadric3D::name(){
    return "blMppGeneratorShapeSuperquadric3D";
}

void blMppGeneratorShapeSuperquadric3D::initParametersWithDefaultsValues(){
    setAxisRange_a(3, 5);
    setAxisRange_b(2, 4);
    setAxisRange_c(2, 3);
    setExposantRange_m(2, 6, 2);
    setExposantRange_n(2, 6, 2);
    setExposantRange_p(2, 6, 2);
    setPhiRange(0, 3.14, 0.2);
    setThetaRange(0, 3.14, 0.2);
    setGammaRange(0, 3.14, 0.2);
}

// setters parameters
void blMppGeneratorShapeSuperquadric3D::setAxisRange_a(int aMin, int aMax){
    m_aMin = aMin;
    m_aMax = aMax;
    m_aRange = m_aMax - m_aMin + 1;

    if (m_aMin <= 0 || m_aMax <= 0 || m_aMin > m_aMax){
        throw( blException("blMppGeneratorShapeSuperquadric3D: a axis setting is not correct"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setAxisRange_b(int bMin, int bMax){
    m_bMin = bMin; m_bMax = bMax;
    m_bRange = m_bMax - m_bMin + 1;
    if (m_bMin <= 0 || m_bMax <= 0 || m_bMin > m_bMax){
        throw( blException("blMppGeneratorShapeSuperquadric3D: b axis setting is not correct"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setAxisRange_c(int cMin, int cMax){
    m_cMin = cMin; m_cMax = cMax;
    m_cRange = m_cMax - m_cMin + 1;
    if (m_cMin <= 0 || m_cMax <= 0 || m_cMin > m_cMax){
        throw( blException("blMppGeneratorShapeSuperquadric3D: c axis setting is not correct"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setExposantRange_m(float mMin, float mMax, float mStep){
    m_mMin = mMin;
    m_mMax = mMax;
    m_mStep = mStep;

    m_exposantXSpace = 0;
    for (float i = m_mMin ; i <= m_mMax ; i+=m_mStep){
        m_exposantXSpace++;
    }

    if (m_exposantXSpace == 0){
        throw( blException("blMppGeneratorShapeSuperquadric3D: There should be at least one m exposant"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setExposantRange_n(float nMin, float nMax, float nStep){
    m_nMin = nMin;
    m_nMax = nMax;
    m_nStep = nStep;

    m_exposantYSpace = 0;
    for (float i = m_nMin ; i <= m_nMax ; i+=m_nStep){
        m_exposantYSpace++;
    }

    if (m_exposantYSpace == 0){
        throw( blException("blMppGeneratorShapeSuperquadric3D: There should be at least one n exposant"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setExposantRange_p(float pMin, float pMax, float pStep){
    m_pMin = pMin;
    m_pMax = pMax;
    m_pStep = pStep;

    m_exposantZSpace = 0;
    for (float i = m_pMin ; i <= m_pMax ; i+=m_pStep){
        m_exposantZSpace++;
    }

    if (m_exposantZSpace == 0){
        throw( blException("blMppGeneratorShapeSuperquadric3D: There should be at least one p exposant"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setPhiRange(float phiMin, float phiMax, float phiStep){
    m_phiMin = phiMin;
    m_phiMax = phiMax;
    m_phiStep = phiStep;

    m_anglesNumberPhi = 0;
    for (float i = m_phiMin ; i <= m_phiMax ; i+=m_phiStep){
        m_anglesNumberPhi++;
    }

    if (m_anglesNumberPhi < 1){
        throw( blException("blMppGeneratorShapeSuperquadric3D: There should be at least one phi angle"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setThetaRange(float thetaMin, float thetaMax, float thetaStep){
    m_thetaMin = thetaMin;
    m_thetaMax = thetaMax;
    m_thetaStep = thetaStep;

    m_anglesNumberTheta = 0;
    for (float i = m_thetaMin ; i <= m_thetaMax ; i+=m_thetaStep){
        m_anglesNumberTheta++;
    }

    if (m_anglesNumberTheta < 1){
        throw( blException("blMppGeneratorShapeSuperquadric3D: There should be at least one theta angle"));
    }
}

void blMppGeneratorShapeSuperquadric3D::setGammaRange(float gammaMin, float gammaMax, float gammaStep){
    m_gammaMin = gammaMin;
    m_gammaMax = gammaMax;
    m_gammaStep = gammaStep;

    m_anglesNumberGamma = 0;
    for (float i = m_gammaMin ; i <= m_gammaMax ; i+=m_gammaStep){
        m_anglesNumberGamma++;
    }

    if (m_anglesNumberGamma < 1){
        throw( blException("blMppGeneratorShapeSuperquadric3D: There should be at least one gamma angle"));
    }
}

// ----------------------- Virtual methods for Shape management ------------

void blMppGeneratorShapeSuperquadric3D::initialize(){

    int sizeLib = m_aRange*m_bRange*m_cRange*
                  m_exposantXSpace*m_exposantYSpace*m_exposantZSpace
                  *m_anglesNumberPhi*m_anglesNumberTheta*m_anglesNumberGamma;
    m_shapeLibrary.resize(sizeLib);

    int b_val, c_val;
    unsigned int iter = -1;
    for (int a = m_aMin ; a <= m_aMax ; ++a){
        for (int b = m_bMin ; b <= m_bMax ; ++b){
            for (int c = m_cMin ; c <= m_cMax ; ++c){
                for (float m = m_mMin ; m <= m_mMax ; m+=m_mStep){
                    for (float n = m_nMin ; n <= m_nMax ; n+=m_nStep){
                        for (float p = m_pMin ; p <= m_pMax ; p+=m_pStep){
                            for (float phi = m_phiMin ; phi <= m_phiMax ; phi += m_phiStep){
                                for (float theta = m_thetaMin ; theta <= m_thetaMax ; theta += m_thetaStep){
                                    for (float gamma = m_gammaMin ; gamma <= m_gammaMax ; gamma += m_gammaStep){
                                        blMppShapeSuperquadric3D *newShape = new blMppShapeSuperquadric3D();
                                        newShape->setCenter(blPixel(0,0,0));

                                        b_val = b;
                                        if (b_val > a){
                                            b_val = a;
                                        }

                                        c_val = c;
                                        if (c_val > b){
                                            c_val = b;
                                        }

                                        //std::cout << "m, n, p  = " << m <<", " << n << ", " << p <<std::endl;
                                        newShape->setParameters(a, b_val, c_val, m, n, p, phi, theta, gamma);
                                        //newShape->printParam();
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //std::cout << "lib size = " << m_shapeLibrary.size() << std::endl;
    this->notify("shape library size = " + blCast::to_string(m_shapeLibrary.size()));
}

blMppShape* blMppGeneratorShapeSuperquadric3D::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSuperquadric3D* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();
    int c = templateShape->c();

    // randomly get center
    int x, y, z;
    int posMin;
    if (templateShape->m() > 2 || templateShape->n() > 2 || templateShape->p() > 2){
        posMin = int(sqrt(a*a + b*b + c*c))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
    int posMaxNl = m_birthArea.xSize() - posMin;
    int posMaxNc = m_birthArea.ySize() - posMin;
    int posMaxNz = m_birthArea.zSize() - posMin;
    bool stop = false;

    while (!stop){
        blPixel pixelIndex = getRandomPixelFromMask();
        if (pixelIndex.x() >= posMin &&  pixelIndex.x() < posMaxNl
                && pixelIndex.y() >= posMin &&  pixelIndex.y() < posMaxNc
                && pixelIndex.z() >= posMin &&  pixelIndex.z() < posMaxNz ){
            x = pixelIndex.x();
            y = pixelIndex.y();
            z = pixelIndex.z();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeSuperquadric3D *newShape = new blMppShapeSuperquadric3D();
    calculateShapeData(blPixel(x,y,z), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeSuperquadric3D::generateRandomUsingArea(){

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSuperquadric3D* templateShape = m_shapeLibrary[pos];
    int a = templateShape->a();
    int b = templateShape->b();
    int c = templateShape->c();

    int posMin;
    if (templateShape->m() > 2 || templateShape->n() > 2 || templateShape->p() > 2){
        posMin = int(sqrt(a*a + b*b + c*c))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
    int rangeMaxNl = m_birthArea.xSize() - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize() - 2*posMin;
    int rangeMaxNz = m_birthArea.zSize() - 2*posMin;
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + float(posMin));
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + float(posMin));
    int z = int(blRandomGeneratorPM::rand()*float(rangeMaxNz) + float(posMin));

    //std::cout << "normal before = " << templateShape->normals()->operator [](0)[0] << ", " << templateShape->normals()->operator [](0)[1] << std::endl;
    blMppShapeSuperquadric3D *newShape = new blMppShapeSuperquadric3D();
    calculateShapeData(blPixel(x,y,z), templateShape, newShape);
    return newShape;
}

void blMppGeneratorShapeSuperquadric3D::calculateShapeData(blPixel centerPixel, blMppShapeSuperquadric3D* templateShape, blMppShapeSuperquadric3D *newShape){

    // parameters
    newShape->setCenter(centerPixel);
    newShape->setParameters(templateShape->a(), templateShape->b(), templateShape->c(),
                            templateShape->m(), templateShape->n(), templateShape->p(),
                            templateShape->phi(), templateShape->theta(), templateShape->gamma());
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

void blMppGeneratorShapeSuperquadric3D::copyNormals(blMppShapeSuperquadric3D* templateShape, blMppShapeSuperquadric3D *newShape){
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

void blMppGeneratorShapeSuperquadric3D::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {

        if ( (*itPixels).z() + t.z() < 0 ){
            std::cout << "translate negative coordinate = " << (*itPixels).z() + t.z() <<  std::endl;
        }
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y(), (*itPixels).z() + t.z());
    }
}

int blMppGeneratorShapeSuperquadric3D::shapesMaxLength(){
    return 2.0*sqrt(m_aMax*m_aMax + m_bMax*m_bMax + m_cMax*m_cMax);
}

// virtual
blMppShape* blMppGeneratorShapeSuperquadric3D::translate(blMppShape* shape){

    //std::cout << "do translation" << std::endl;
    blMppShapeSuperquadric3D* shapesq = dynamic_cast<blMppShapeSuperquadric3D*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int deltaZ = int(blMath::randSign())*m_deltaTranslation;
    int x = shapesq->getCenter().x() + deltaX;
    int y = shapesq->getCenter().y() + deltaY;
    int z = shapesq->getCenter().z() + deltaZ;


    // control if still inside image
    int posMin;
    int a = shapesq->a();
    int b = shapesq->b();
    int c = shapesq->c();
    if (shapesq->m() > 2 || shapesq->n() > 2 || shapesq->n() > 2){
        posMin = int(sqrt(a*a + b*b + c*c))+ m_borderWidth;
    }
    else{
        posMin = a + m_borderWidth;
    }
    int posMaxNl = m_birthArea.xSize() - posMin;
    int posMaxNc = m_birthArea.ySize() - posMin;
    int posMaxNz = m_birthArea.zSize() - posMin;

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
    if (z <= posMin){
        z = shapesq->getCenter().z();
        deltaZ = 0;
    }
    if (z >= posMaxNz){
        z = shapesq->getCenter().z();
        deltaZ = 0;
    }

    // fill shape
    blMppShapeSuperquadric3D *newShape = new blMppShapeSuperquadric3D();
    // copy parameters
    newShape->setCenter(blPixel(x,y,z));
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->c(),
                            shapesq->m(), shapesq->n(), shapesq->p(),
                            shapesq->phi(), shapesq->theta(), shapesq->gamma());
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

blMppShape* blMppGeneratorShapeSuperquadric3D::rotate(blMppShape* shape){

    //std::cout << "do rotate" << std::endl;
    // get a perturbated angle
    blMppShapeSuperquadric3D* shapesq = dynamic_cast<blMppShapeSuperquadric3D*>(shape);

    float anglePhi = shapesq->phi() + blMath::randSign()*m_deltaRotation;
    if (anglePhi < m_phiMin){
        anglePhi = m_phiMin;
    }
    if (anglePhi > m_phiMax){
        anglePhi = m_phiMax;
    }

    float angleTheta = shapesq->theta() + blMath::randSign()*m_deltaRotation;
    if (angleTheta < m_thetaMin){
        angleTheta = m_thetaMin;
    }
    if (angleTheta > m_thetaMax){
        angleTheta = m_thetaMax;
    }

    float angleGamma = shapesq->gamma() + blMath::randSign()*m_deltaRotation;
    if (angleGamma < m_gammaMin){
        angleGamma = m_gammaMin;
    }
    if (angleGamma > m_gammaMax){
        angleGamma = m_gammaMax;
    }

    // find the appropriate template
    int posa = shapesq->a() - m_aMin;
    int posb = shapesq->b() - m_bMin;
    int posc = shapesq->c() - m_cMin;
    int posm = int((shapesq->m() - m_mMin)/m_mStep);
    int posn = int((shapesq->n() - m_nMin)/m_nStep);
    int posp = int((shapesq->p() - m_pMin)/m_pStep);
    int posPhi = int((anglePhi - m_phiMin)/m_phiStep);
    int posTheta = int((angleTheta - m_thetaMin)/m_thetaStep);
    int posGamma = int((angleGamma - m_gammaMin)/m_gammaStep);

    int cumulSize = 1;

    cumulSize*=m_anglesNumberGamma; int id1 = cumulSize*posTheta+posGamma;
    cumulSize*=m_anglesNumberTheta; int id2 = cumulSize*posPhi+id1;
    cumulSize*=m_anglesNumberPhi;   int id3 = cumulSize*posp+id2;
    cumulSize*=m_exposantZSpace;    int id4 = cumulSize*posn+id3;
    cumulSize*=m_exposantYSpace;    int id5 = cumulSize*posm+id4;
    cumulSize*=m_exposantXSpace;    int id6 = cumulSize*posc+id5;
    cumulSize*=m_cRange;            int id7 = cumulSize*posb+id6;
    cumulSize*=m_bRange;            int idx = cumulSize*posa+id7;

    // create the new shape
    blMppShapeSuperquadric3D *newShape = new blMppShapeSuperquadric3D();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(shapesq->a(), shapesq->b(), shapesq->c(),
                            shapesq->m(), shapesq->n(), shapesq->p(),
                            anglePhi, angleTheta, angleGamma );
    newShape->setMaxDistanceToBarycenter(shapesq->maxDistanceToBarycenter());

    // set silhouette
    blMppShapeSuperquadric3D *shapeTemplate = m_shapeLibrary[idx];
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

blMppShape* blMppGeneratorShapeSuperquadric3D::resize(blMppShape* shape){

    //std::cout << "do resize" << std::endl;
    blMppShapeSuperquadric3D* shapesq = dynamic_cast<blMppShapeSuperquadric3D*>(shape);

    // get a perturbated angle
    int a = shapesq->a() + blMath::randSign()*m_deltaResize;
    int b = shapesq->b() + blMath::randSign()*m_deltaResize;
    int c = shapesq->c() + blMath::randSign()*m_deltaResize;
    if (a > m_aMax){ a = m_aMax;}
    if (a < m_aMin){ a = m_aMin;}
    if (b > m_bMax){ b = m_bMax;}
    if (b < m_bMin){ b = m_bMin;}
    if (c > m_cMax){ c = m_cMax;}
    if (c < m_cMin){ c = m_cMin;}

    // refuse resize if the object goes out of the image
    if ( shapesq->getCenter().x() + a + m_borderWidth >= m_birthArea.xSize()
         || shapesq->getCenter().x() - a - m_borderWidth <= 0
         || shapesq->getCenter().y() + a + m_borderWidth >= m_birthArea.ySize()
         || shapesq->getCenter().y() - a - m_borderWidth <= 0
         || shapesq->getCenter().z() + a + m_borderWidth >= m_birthArea.zSize()
         || shapesq->getCenter().z() - a - m_borderWidth <= 0
         ){
        a = shapesq->a();
        if (b > a){
            b = a;
        }
    }

    // find the appropriate template
    int posa = a - m_aMin;
    int posb = b - m_bMin;
    int posc = c - m_cMin;
    int posm = int((shapesq->m() - m_mMin)/m_mStep);
    int posn = int((shapesq->n() - m_nMin)/m_nStep);
    int posp = int((shapesq->p() - m_pMin)/m_pStep);
    int posPhi = int((shapesq->phi() - m_phiMin)/m_phiStep);
    int posTheta = int((shapesq->theta() - m_thetaMin)/m_thetaStep);
    int posGamma = int((shapesq->gamma() - m_gammaMin)/m_gammaStep);

    int cumulSize = 1;

    cumulSize*=m_anglesNumberGamma; int id1 = cumulSize*posTheta+posGamma;
    cumulSize*=m_anglesNumberTheta; int id2 = cumulSize*posPhi+id1;
    cumulSize*=m_anglesNumberPhi;   int id3 = cumulSize*posp+id2;
    cumulSize*=m_exposantZSpace;    int id4 = cumulSize*posn+id3;
    cumulSize*=m_exposantYSpace;    int id5 = cumulSize*posm+id4;
    cumulSize*=m_exposantXSpace;    int id6 = cumulSize*posc+id5;
    cumulSize*=m_cRange;            int id7 = cumulSize*posb+id6;
    cumulSize*=m_bRange;            int idx = cumulSize*posa+id7;

    // create the new shape
    blMppShapeSuperquadric3D *newShape = new blMppShapeSuperquadric3D();
    newShape->setCenter(shapesq->getCenter());
    newShape->setParameters(a, b, c,
                            shapesq->m(), shapesq->n(), shapesq->p(),
                            shapesq->phi(), shapesq->theta(), shapesq->gamma()  );
    newShape->computeMaxDistanceToBarycenter();

    // set silhouette
    blMppShapeSuperquadric3D *shapeTemplate = m_shapeLibrary[idx];
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
