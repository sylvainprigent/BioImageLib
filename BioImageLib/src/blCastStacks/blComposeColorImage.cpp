/// \file blComposeColorImage.cpp
/// \brief blComposeColorImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blComposeColorImage.h"

blComposeColorImage::blComposeColorImage() : blProcess(){

}

blComposeColorImage::~blComposeColorImage(){

}

std::string blComposeColorImage::name(){
    return "blComposeColorImage";
}

void blComposeColorImage::setComposant1(blImage* comp1){
    m_comp1 = comp1;
}

void blComposeColorImage::setComposant2(blImage* comp2){
    m_comp2 = comp2;
}

void blComposeColorImage::setComposant3(blImage* comp3){
    m_comp3 = comp3;
}

void blComposeColorImage::run(){
    if (m_comp1->imageType() == blImage::TypeInt2D
            && m_comp2->imageType() == blImage::TypeInt2D
            && m_comp3->imageType() == blImage::TypeInt2D ){

        m_outputImage = composeImages<Int2DImage, IntColor2DImage>( m_comp1->itkInt2DImagePointer(),
                              m_comp2->itkInt2DImagePointer(),
                              m_comp3->itkInt2DImagePointer());
    }
    else if (m_comp1->imageType() == blImage::TypeFloat2D
            && m_comp2->imageType() == blImage::TypeFloat2D
            && m_comp3->imageType() == blImage::TypeFloat2D ){

        m_outputImage = composeImages<Float2DImage, FloatColor2DImage>( m_comp1->itkFloat2DImagePointer(),
                              m_comp2->itkFloat2DImagePointer(),
                              m_comp3->itkFloat2DImagePointer());
    }
    else if (m_comp1->imageType() == blImage::TypeInt3D
            && m_comp2->imageType() == blImage::TypeInt3D
            && m_comp3->imageType() == blImage::TypeInt3D ){

        m_outputImage = composeImages<Int3DImage, IntColor3DImage>( m_comp1->itkInt3DImagePointer(),
                              m_comp2->itkInt3DImagePointer(),
                              m_comp3->itkInt3DImagePointer());
    }
    else if (m_comp1->imageType() == blImage::TypeFloat3D
            && m_comp2->imageType() == blImage::TypeFloat3D
            && m_comp3->imageType() == blImage::TypeFloat3D ){

        m_outputImage = composeImages<Float3DImage, FloatColor3DImage>( m_comp1->itkFloat3DImagePointer(),
                              m_comp2->itkFloat3DImagePointer(),
                              m_comp3->itkFloat3DImagePointer());
    }
    else{
        std::string message = "blComposeColorImage con combine only scalar images of same type";
        throw blException(message.c_str());
    }
}

blImage* blComposeColorImage::output(){
    return m_outputImage;
}
