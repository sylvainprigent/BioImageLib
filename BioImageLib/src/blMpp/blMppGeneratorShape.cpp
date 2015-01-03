/// \file blMppGeneratorShape.cpp
/// \brief blMppGeneratorShape class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShape.h"
#include <blCore>

// itk
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "blRand/blRandomGeneratorPM.h"

using namespace std;

// ////////////////////////////////////////////////////////////////// //
//                        blMppGeneratorShape
// ////////////////////////////////////////////////////////////////// //
blMppGeneratorShape::blMppGeneratorShape(): blProcess(){
    initAttributs();
}


blMppGeneratorShape::blMppGeneratorShape(blImage* birthMap): blProcess(){
    initAttributs();
    setBirthMap(birthMap);
}

blMppGeneratorShape::blMppGeneratorShape(blImageSize size): blProcess(){
    initAttributs();
    m_birthArea = size;
}

void blMppGeneratorShape::initAttributs(){

    m_borderWidth = 1;
    m_useBirthMap = false;

    m_deltaTranslation = 1;
    m_deltaRotation = 0.2;
    m_deltaResize = 1;
    m_ProbaPerutbation1 = 0.3;
    m_ProbaPerutbation2 = 0.6;

    m_calculateBorderPixels = false;
    m_calculateInsidePixels = false;
    m_calculateNormals = false;

    m_is3DShapes = false;
}


blMppGeneratorShape::~blMppGeneratorShape(){

}

bool blMppGeneratorShape::is3DShape(){
    return m_is3DShapes;
}

// //////////////////////////////// parameters /////////////////////////////////////////////
void blMppGeneratorShape::setBorderWidth(int width){
    m_borderWidth = width;
}

void blMppGeneratorShape::setDeltaTranslation(int value){
    m_deltaTranslation = value;
}

void blMppGeneratorShape::setDeltaRotation(float value){
    m_deltaRotation = value;
}

void blMppGeneratorShape::setDeltaResize(int value){
    m_deltaResize = value;
}

void blMppGeneratorShape::setCalculateBorderPixels(bool value){
    m_calculateBorderPixels = value;

}

void blMppGeneratorShape::setCalculateInsidePixels(bool value){
    m_calculateInsidePixels = value;

}

void blMppGeneratorShape::setCalculateNormals(bool value){
    m_calculateNormals = value;
}

// generate
blMppShape *blMppGeneratorShape::generateRandom(){
    if (m_useBirthMap){
        return generateRandomUsingMask();
    }
    else{
        return generateRandomUsingArea();
    }
}

// /////////////////////////////// getters/setters ////////////////////////////////
void blMppGeneratorShape::setBirthMap(blImage* birthMap){
    m_useBirthMap = true;
    m_birthArea.setXSize(birthMap->imageSize().xSize());
    m_birthArea.setYSize(birthMap->imageSize().ySize());
    m_birthArea.setZSize(birthMap->imageSize().zSize());

    if (birthMap->is3D()){
        if (birthMap->imageType() == blImage::TypeInt3D){
            Int3DImage::IndexType pixelIndex;
            Int3DImage::Pointer maskImagePointer = birthMap->itkInt3DImagePointer();
            //m_birthMap.reserve(birthMap->imageSize().xSize()*birthMap->imageSize().ySize()/2);
            for (int m = 0 ; m < birthMap->imageSize().xSize(); ++m){
                for (int n = 0 ; n < birthMap->imageSize().ySize() ; ++n){
                    for (int p = 0 ; p < birthMap->imageSize().zSize() ; ++p){
                        pixelIndex[0] = m;
                        pixelIndex[1] = n;
                        pixelIndex[2] = p;
                        float pixelValue = maskImagePointer->GetPixel(pixelIndex);
                        if (pixelValue == 0 || pixelValue == 255){
                            if (pixelValue == 255){
                                m_birthMap.push_back(blPixel(m,n,p));
                            }
                        }
                        else{
                            throw blException("blMppGeneratorShape: The birth map must be binary {0,255}");
                        }
                    }
                }
            }
        }
        else{
            throw blException("blMppGeneratorShape: The birth map must be a UChar images mask");
        }

    }
    else{
        if (birthMap->imageType() == blImage::TypeInt2D){
            Int2DImage::IndexType pixelIndex;
            Int2DImage::Pointer maskImagePointer = birthMap->itkInt2DImagePointer();
            m_birthMap.reserve(birthMap->imageSize().xSize()*birthMap->imageSize().ySize()/2);
            for (int m = 0 ; m < birthMap->imageSize().xSize(); ++m){
                for (int n = 0 ; n < birthMap->imageSize().ySize() ; ++n){
                    pixelIndex[0] = m;
                    pixelIndex[1] = n;
                    float pixelValue = maskImagePointer->GetPixel(pixelIndex);
                    if (pixelValue == 0 || pixelValue == 255){
                        if (pixelValue == 255){
                            m_birthMap.push_back(blPixel(m,n));
                        }
                    }
                    else{
                        throw blException("blMppGeneratorShape: The birth map must be binary {0,255}");
                    }
                }
            }
        }
        else{
            throw blException("blMppGeneratorShape: The birth map must be a UChar images mask");
        }
    }
}

blPixel blMppGeneratorShape::getRandomPixelFromMask(){

    float index = int(blRandomGeneratorPM::rand()*float(m_birthMap.size()));
    return m_birthMap[index];
}

void blMppGeneratorShape::setBirthArea(blImageSize area){
    m_birthArea = area;
}


string blMppGeneratorShape::name(){
    return "blMppGeneratorShape";
}

bool blMppGeneratorShape::useBirthMap(){
    return m_useBirthMap;
}

int blMppGeneratorShape::shapesMaxLengthZ(){
    return 0;
}

// perturbations
void blMppGeneratorShape::setPerturbationsProba(float translation, float resize, float rotate){
    float difference = translation + resize + rotate - 1.0;
    if (difference <= std::numeric_limits<float>::epsilon()){
        m_ProbaPerutbation1 = translation;
        m_ProbaPerutbation2 = translation+resize;
    }
    else{
        throw blException("blMppAlgorithmMBCR -> translation, rotation and resize proba must sum to 1");
    }
}

blMppShape* blMppGeneratorShape::generatePerturbation(blMppShape* shape){
    float rt = blRandomGeneratorPM::rand();
    blMppShape* new_shape;
    if (rt < m_ProbaPerutbation1){
        new_shape = this->translate(shape);
    }
    else if (rt >= m_ProbaPerutbation1 && rt < m_ProbaPerutbation2){
        new_shape = this->resize(shape);
    }
    else{
        new_shape = this->rotate(shape);
    }
    return new_shape;
}
