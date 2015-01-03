/// \file blMppDataTermGradientNormalized.cpp
/// \brief blMppDataTermGradientNormalized class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppDataTermGradientNormalized.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

using namespace std;
#include <math.h>
#include <blMath>

blMppDataTermGradientNormalized::blMppDataTermGradientNormalized() : blMppDataTerm(){
    m_epsilon = 0.000001;
    m_sigma = 1.5;
    m_gradientDirection = 1;
    m_gradient2D = NULL;
    m_gradient3D = NULL;
    thresholdPtr = NULL;
    m_is2DImage = true;
}

blMppDataTermGradientNormalized::blMppDataTermGradientNormalized(blImage* image)
    : blMppDataTerm(image){
    m_epsilon = 0.000001;
    m_sigma = 1.5;
    m_gradientDirection = 1;
    m_gradient2D = NULL;
    m_gradient3D = NULL;
    thresholdPtr = NULL;
    m_is2DImage = true;
}

blMppDataTermGradientNormalized::~blMppDataTermGradientNormalized(){

}

// blProcess methods
std::string blMppDataTermGradientNormalized::name(){
    return "blMppDataTermGradientNormalized";
}


// setters parameters
void blMppDataTermGradientNormalized::setEpsilon(float epsilon){
    m_epsilon = epsilon;
}

void blMppDataTermGradientNormalized::setSigma(float sigma){
    m_sigma = sigma;
}

void blMppDataTermGradientNormalized::setGradientDirection(int gradientDirection){
    m_gradientDirection = gradientDirection;
    if ( gradientDirection != 0 && gradientDirection != -1 && gradientDirection != 1){
        throw( blException("blMppDataTermGradientNormalized: gradient direction must be -1, 0, or 1"));
    }
}

void blMppDataTermGradientNormalized::initialize(){

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

void blMppDataTermGradientNormalized::calculateGradientImage(){
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

float blMppDataTermGradientNormalized::compute(blMppShape* shape){
    if (m_is2DImage){
        return compute2D(shape);
    }
    else{
        return compute3D(shape);
    }
}

float blMppDataTermGradientNormalized::compute2D(blMppShape* shape){

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

        float norm = grad_x*grad_x + grad_y*grad_y;
        float normCoef = sqrt(norm + m_epsilon*m_epsilon);

        // project gradient
        nx = normalsBorder->operator [](p)[0];
        ny = normalsBorder->operator [](p)[1];
        //std::cout << "nx = " << nx << "ny = " << ny << std::endl;
        float val = nx*grad_x/normCoef + ny*grad_y/normCoef;


        if (!blMath::isNan(val))
            U += val;
    }
    U /= borderPixels->size();

    // threshold energy
    return (this->*thresholdPtr)(U);
}

float blMppDataTermGradientNormalized::compute3D(blMppShape* shape){

    Float3DImage::IndexType pixelIndex;
    vector<blPixel>* borderPixels = shape->borderPixels();
    vector<vector<float> >* normalsBorder = shape->normalsBorder();
    float U = 0;
    itk::CovariantVector< float, 3  > grad;
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

        float norm = grad_x*grad_x + grad_y*grad_y + grad_z*grad_z;
        float normCoef = sqrt(norm + m_epsilon*m_epsilon);

        // project gradient
        nx = normalsBorder->operator [](p)[0];
        ny = normalsBorder->operator [](p)[1];
        nz = normalsBorder->operator [](p)[2];
        //std::cout << "nx = " << nx << "ny = " << ny << std::endl;
        float val = (nx*grad_x + ny*grad_y + nz*grad_z )/normCoef;


        if (!blMath::isNan(val))
            U += val;
    }
    U /= borderPixels->size();

    // threshold energy
    return (this->*thresholdPtr)(U);
}

void blMppDataTermGradientNormalized::createCompatibleShapesList(){
    m_compatibleShapesList.push_back("all");
}

bool blMppDataTermGradientNormalized::useInsidePixels(){
    return false;
}

bool blMppDataTermGradientNormalized::useBorderPixels(){
    return true;
}

bool blMppDataTermGradientNormalized::useNormals(){
    return true;
}

// gradient threshold
void blMppDataTermGradientNormalized::initGradientFunctionsPointers(){
    // Select the threshold function due to the gradient parameter
    if (m_gradientDirection == -1){
        thresholdPtr = &blMppDataTermGradientNormalized::thresholdNegatif;
    }
    else if (m_gradientDirection == 1){
        thresholdPtr = &blMppDataTermGradientNormalized::thresholdPositif;
    }
    else if (m_gradientDirection == 0){
        thresholdPtr = &blMppDataTermGradientNormalized::thresholdsymetric;
    }
}

float blMppDataTermGradientNormalized::thresholdNegatif(float energy){
    float den = float(m_threshold)+1.0;
    return min( float((-energy-float(m_threshold))/den), float(1.0) );
}

float blMppDataTermGradientNormalized::thresholdPositif(float energy){
    float den = float(m_threshold)+1.0;
    return min( float((energy-float(m_threshold))/den ) , float(1.0));
}

float blMppDataTermGradientNormalized::thresholdsymetric(float energy){
    float den = float(m_threshold)+1.0;
    float f1 = energy/den;
    float f2 = -energy/den;
    return min(f1,f2) - float(m_threshold)/den;
}
