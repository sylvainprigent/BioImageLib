/// \file blMppDataTermGradient.h
/// \brief blMppDataTermGradient class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppDataTerm.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkImage.h"

/// \class blMppDataTermGradient
/// \brief Class that define an Marked Point Process data term
/// as the average gradient of the pixels in the border of a shape
class blMppDataTermGradient : public blMppDataTerm{

public:
    /// \fn blMppDataTermGradient();
    /// \brief reimplement from blMppDataTerm
    blMppDataTermGradient();
    /// \fn blMppDataTermGradient(blImage* image);
    /// \brief reimplement from blMppDataTerm
    blMppDataTermGradient(blImage* image);
    /// \fn virtual ~blMppDataTermGradient();
    /// \brief reimplement from blMppDataTerm
    virtual ~blMppDataTermGradient();

public:
    /// \fn virtual void initialize();
    /// \brief reimplement from blMppDataTerm
    virtual void initialize();
    /// \fn virtual float compute(blMppShape* shape);
    /// \brief reimplement from blMppDataTerm
    virtual float compute(blMppShape* shape);
    /// \fn virtual void createCompatibleShapesList();
    /// \brief reimplement from blMppDataTerm
    virtual void createCompatibleShapesList();

    /// \fn virtual bool useInsidePixels();
    /// \brief reimplement from blMppDataTerm
    virtual bool useInsidePixels();
    /// \fn virtual bool useBorderPixels();
    /// \brief reimplement from blMppDataTerm
    virtual bool useBorderPixels();
    /// \fn virtual bool useNormals();
    /// \brief reimplement from blMppDataTerm
    virtual bool useNormals();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplement from blMppDataTerm
    virtual std::string name();

public:
    // setters parameters
    /// \fn void setSigma(float sigma);
    /// \param[in] sigma Sigma used to calculate
    /// the gradient using the iterative Gaussian gradient
    void setSigma(float sigma);
    /// \fn void setGradientDirection(int gradientDirection);
    /// \param[in] gradientDirection Color of the object
    ///          - (1) Light shapes in dark background
    ///          - (-1)Dark shapes in light background
    ///          - (0) Both light and dark shapes
    void setGradientDirection(int gradientDirection);
    /// \fn void setD(float D);
    /// \param[in] D Slope of the exponential function that
    /// fit the gradient value into [-1,1]
    void setD(float D);

protected:
    int m_gradientDirection; ///<
    float m_sigma; ///<
    float m_D; ///<

protected:
    bool m_is2DImage;

protected:
    // internal Data
    itk::Image< itk::CovariantVector< float, 2 >, 2 >::Pointer m_gradient2D; ///<
    itk::Image< itk::CovariantVector< float, 3 >, 3 >::Pointer m_gradient3D; ///<


protected:
    // functions pointers for threshold
    typedef float (blMppDataTermGradient::*ThresholdFunction)(float); ThresholdFunction thresholdPtr;  
    float thresholdNegatif(float energy);
    float thresholdPositif(float energy);
    float thresholdsymetric(float energy);
    void initGradientFunctionsPointers();
    float thresholdEnergy(float energy);

    /// \brief Compute method for the 2D case
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    float compute2D(blMppShape* shape);
    /// \brief Compute method for the 3D case
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    float compute3D(blMppShape *shape);
    /// \brief Commpute the gradient of the image
    /// depending on the 2D or 3D case
    void calculateGradientImage();
};
