/// \file blDiscreteGaussianFilter.h
/// \brief blDiscreteGaussianFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkDiscreteGaussianImageFilter.h"

/// \class blDiscreteGaussianFilter
/// \brief Blurs an image by separable convolution with discrete gaussian kernels.
/// This filter performs Gaussian blurring by separable convolution of an image
/// and a discrete Gaussian operator (kernel).
class blDiscreteGaussianFilter : public blFilter{

public:
    /// \fn blDiscreteGaussianFilter();
    /// \brief Constructor
    blDiscreteGaussianFilter();
    /// \fn virtual ~blDiscreteGaussianFilter();
    /// \brief Destructor
    virtual ~blDiscreteGaussianFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // IO
    /// \fn void run();
    /// \brief Implement from blFilter
    void run();

    // parameters
    /// \fn void setVariance(float variance);
    /// \param[in] variance Variance of the gaussian kernel
    void setVariance(float variance);

private:
    float m_variance;

private:
    template<typename InputImageType>
    blImage* discreteGaussian(typename InputImageType::Pointer image){
        typedef itk::DiscreteGaussianImageFilter<
          InputImageType, InputImageType >  FilterType;

        typename FilterType::Pointer discreteGaussianImageFilter = FilterType::New();
        discreteGaussianImageFilter->SetInput(image);
        discreteGaussianImageFilter->SetVariance(m_variance);
        discreteGaussianImageFilter->Update();
        return new blImage(discreteGaussianImageFilter->GetOutput());
    }
};
