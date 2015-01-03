/// \file blRecursiveGaussianFilter.h
/// \brief blRecursiveGaussianFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"

/// \class blRecursiveGaussianFilter
/// \brief Computes the smoothing of an image by convolution with the
/// Gaussian kernels implemented as IIR filters.
/// This filter is implemented using the recursive gaussian filters.
/// For multi-component images, the filter works on each component independently.
class blRecursiveGaussianFilter : public blFilter{

public:
    /// \fn blRecursiveGaussianFilter();
    /// \brief Constructor
    blRecursiveGaussianFilter();
    /// \fn virtual ~blRecursiveGaussianFilter();
    /// \brief Destructor
    virtual ~blRecursiveGaussianFilter();

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
    /// \fn void setSigma(float sigma);
    /// \param[in] sigma Standard deviation of the gaussian kernel
    void setSigma(float sigma);

private:
    // parameters
    float m_sigma;

private:
    // internal methods
    template<typename InputImageType>
    blImage* recursiveGaussian(typename InputImageType::Pointer image){
        typedef itk::SmoothingRecursiveGaussianImageFilter<
          InputImageType, InputImageType >  FilterType;

        typename FilterType::Pointer smoothingRecursiveGaussianImageFilter = FilterType::New();
        smoothingRecursiveGaussianImageFilter->SetInput(image);
        smoothingRecursiveGaussianImageFilter->SetSigma(m_sigma);
        smoothingRecursiveGaussianImageFilter->Update();
        return new blImage(smoothingRecursiveGaussianImageFilter->GetOutput());
    }
};
