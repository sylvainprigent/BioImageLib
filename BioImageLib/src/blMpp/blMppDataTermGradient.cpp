/// \file blMppDataTermGradient.cpp
/// \brief blMppDataTermGradient class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppDataTermGradient.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

using namespace std;
#include <blMath>

blMppDataTermGradient::blMppDataTermGradient() : blMppDataTerm(){
    m_D = 100.0;
    m_sigma = 1.5;
    m_gradientDirection = 1;
    m_gradient2D = NULL;
    m_gradient3D = NULL;
    thresholdPtr = NULL;
    m_is2DImage = true;
}

blMppDataTermGradient::blMppDataTermGradient(blImage* image)
    : blMppDataTerm(image){
    m_D = 100.0;
    m_sigma = 1.5;
    m_gradientDirection = 1;
    m_gradient2D = NULL;
    m_gradient3D = NULL;
    thresholdPtr = NULL;
    m_is2DImage = true;
}

blMppDataTermGradient::~blMppDataTermGradient(){

}

// blProcess methods
std::string blMppDataTermGradient::name(){
    return "blMppDataTermGradient";
}


// setters parameters
void blMppDataTermGradient::setSigma(float sigma){
    m_sigma = sigma;
}

void blMppDataTermGradient::setGradientDirection(int gradientDirection){
    m_gradientDirection = gradientDirection;
    if ( gradientDirection != 0 && gradientDirection != -1 && gradientDirection != 1){
        throw( blException("blMppDataTermGradient: gradient direction must be -1, 0, or 1"));
    }
}

void blMppDataTermGradient::setD(float D){
    m_D = D;
}

void blMppDataTermGradient::initialize(){

    // 2D vs 3D
    if (m_image->imageType() == blImage::TypeFloat2D){
        m_is2DImage = true;
    }
    else if (m_image->imageType() == blImage::TypeFloat3D){
        m_is2DImage = false;
    }
    else{
        throw blException("blMppDataTermBhattacharyya can process only images of type Float2DImage");
    }

    // gradient image
    calculateGradientImage();

    // init threshold function
    initGradientFunctionsPointers();
}

void blMppDataTermGradient::calculateGradientImage(){
    if (m_is2DImage){
        // compute the gradient image
        typedef itk::CovariantVector< float, 2  > OutputPixelType;
        typedef itk::Image< float,  2 > InputImageType;
        typedef itk::Image< OutputPixelType, 2 > OutputImageType;
        typedef itk::GradientRecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;

        FilterType::Pointer filter = FilterType::New();
        filter->SetSigma( m_sigma );
        filter->SetInput( m_image->itkFloat2DImagePointer() );
        filter->Update();
        m_gradient2D = filter->GetOutput();
    }
    else{
        // compute the gradient image
        typedef itk::CovariantVector< float, 3  > OutputPixelType;
        typedef itk::Image< float,  3 > InputImageType;
        typedef itk::Image< OutputPixelType, 3 > OutputImageType;
        typedef itk::GradientRecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;

        FilterType::Pointer filter = FilterType::New();
        filter->SetSigma( m_sigma );
        filter->SetInput( m_image->itkFloat3DImagePointer() );
        filter->Update();
        m_gradient3D = filter->GetOutput();
    }
}

float blMppDataTermGradient::compute(blMppShape* shape){

    if (m_is2DImage){
        return compute2D(shape);
    }
    else{
        return compute3D(shape);
    }
}

float blMppDataTermGradient::compute2D(blMppShape* shape){

    Float2DImage::IndexType pixelIndex;
    vector<blPixel>* borderPixels = shape->borderPixels();
    vector<vector<float> >* normalsBorder = shape->normalsBorder();
    float U = 0;
    itk::CovariantVector< float, 2  > grad;
    float grad_x, grad_y;
    float nx, ny;
    for (int p = 0 ; p < borderPixels->size() ; ++p){
        blPixel curentPixel = borderPixels->operator [](p);

        // get gradient
        pixelIndex[0] = curentPixel.x();
        pixelIndex[1] = curentPixel.y();
        grad = m_gradient2D->GetPixel(pixelIndex);
        grad_x = grad[0];
        grad_y = grad[1];

        // project gradient
        nx = normalsBorder->operator [](p)[0];
        ny = normalsBorder->operator [](p)[1];
        //std::cout << "nx = " << nx << "ny = " << ny << std::endl;
        float val = nx*grad_x + ny*grad_y;

        if (!blMath::isNan(val))
            U += val;
    }
    U /= borderPixels->size();

    // threshold energy
    return (this->*thresholdPtr)(U);
}

float blMppDataTermGradient::compute3D(blMppShape* shape){

    Float3DImage::IndexType pixelIndex;
    vector<blPixel>* borderPixels = shape->borderPixels();
    vector<vector<float> >* normalsBorder = shape->normalsBorder();
    float U = 0;
    itk::CovariantVector< float, 3 > grad;
    float grad_x, grad_y, grad_z;
    float nx, ny, nz;
    for (int p = 0 ; p < borderPixels->size() ; ++p){
        blPixel curentPixel = borderPixels->operator [](p);

        // get gradient
        pixelIndex[0] = curentPixel.x();
        pixelIndex[1] = curentPixel.y();
        pixelIndex[2] = curentPixel.z();
        grad = m_gradient3D->GetPixel(pixelIndex);
        grad_x = grad[0];
        grad_y = grad[1];
        grad_z = grad[2];

        // project gradient
        nx = normalsBorder->operator [](p)[0];
        ny = normalsBorder->operator [](p)[1];
        nz = normalsBorder->operator [](p)[2];
        //std::cout << "nx = " << nx << "ny = " << ny << std::endl;
        float val = nx*grad_x + ny*grad_y + nz*grad_z;

        if (!blMath::isNan(val))
            U += val;
    }
    U /= borderPixels->size();

    // threshold energy
    return (this->*thresholdPtr)(U);
}

void blMppDataTermGradient::createCompatibleShapesList(){
    m_compatibleShapesList.push_back("all");
}

bool blMppDataTermGradient::useInsidePixels(){
    return false;
}

bool blMppDataTermGradient::useBorderPixels(){
    return true;
}

bool blMppDataTermGradient::useNormals(){
    return true;
}

// gradient threshold
void blMppDataTermGradient::initGradientFunctionsPointers(){
    // Select the threshold function due to the gradient parameter
    if (m_gradientDirection == -1){
        thresholdPtr = &blMppDataTermGradient::thresholdNegatif;
    }
    else if (m_gradientDirection == 1){
        thresholdPtr = &blMppDataTermGradient::thresholdPositif;
    }
    else if (m_gradientDirection == 0){
        thresholdPtr = &blMppDataTermGradient::thresholdsymetric;
    }
}

float blMppDataTermGradient::thresholdNegatif(float energy){
    if (energy < 0){
        energy = 0.0;
    }
    return thresholdEnergy(energy);
}

float blMppDataTermGradient::thresholdPositif(float energy){
    if (energy > 0){
        energy = 0.0;
    }
    return thresholdEnergy(-energy);
}

float blMppDataTermGradient::thresholdsymetric(float energy){

    if (energy < 0 ){
        return thresholdEnergy(-energy);
    }
    else{
        return thresholdEnergy(energy);
    }
}

float blMppDataTermGradient::thresholdEnergy(float energy){
    if (energy < m_threshold){
        return (1- energy/m_threshold);
    }
    else{
        return exp(-(energy - m_threshold)/m_D)-1;
    }
}
